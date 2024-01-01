/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2024 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err_utils.h>

/*
 * This example was shamelessly stoen from:
 * https://unixism.net/loti/low_level.html
 * It shows how to use the io_uring low leval API to implement a cat(1) like program
 */

/* If your compilation fails because the header file below is missing,
* your kernel is probably too old to support io_uring.
* */
#include <linux/io_uring.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ 1024

/* This is x86 specific */
#define read_barrier() __asm__ __volatile__("":::"memory")
#define write_barrier() __asm__ __volatile__("":::"memory")

struct app_io_sq_ring {
	unsigned *head;
	unsigned *tail;
	unsigned *ring_mask;
	unsigned *ring_entries;
	unsigned *flags;
	unsigned *array;
};

struct app_io_cq_ring {
	unsigned *head;
	unsigned *tail;
	unsigned *ring_mask;
	unsigned *ring_entries;
	struct io_uring_cqe *cqes;
};

struct submitter {
	int ring_fd;
	struct app_io_sq_ring sq_ring;
	struct io_uring_sqe *sqes;
	struct app_io_cq_ring cq_ring;
};

struct file_info {
	off_t file_sz;
	struct iovec* iovecs; /* Referred by readv/writev */
};

/*
* This code is written in the days when io_uring-related system calls are not
* part of standard C libraries. So, we roll our own system call wrapper
* functions.
* */

int io_uring_setup(unsigned entries, struct io_uring_params *p)
{
	return (int) syscall(__NR_io_uring_setup, entries, p);
}

int io_uring_enter(int ring_fd, unsigned int to_submit,
	unsigned int min_complete, unsigned int flags)
{
	return (int) syscall(__NR_io_uring_enter, ring_fd, to_submit, min_complete, flags, NULL, 0);
}

/*
* Returns the size of the file whose open file descriptor is passed in.
* Properly handles regular file and block devices as well. Pretty.
* */

off_t get_file_size(int fd) {
	struct stat st;

	CHECK_NOT_M1(fstat(fd, &st));
	if (S_ISBLK(st.st_mode)) {
		unsigned long long bytes;
		CHECK_NOT_M1(ioctl(fd, BLKGETSIZE64, &bytes));
		return bytes;
	} else if (S_ISREG(st.st_mode))
		return st.st_size;
	return -1;
}

/*
* io_uring requires a lot of setup which looks pretty hairy, but isn't all
* that difficult to understand. Because of all this boilerplate code,
* io_uring's author has created liburing, which is relatively easy to use.
* However, you should take your time and understand this code. It is always
* good to know how it all works underneath. Apart from bragging rights,
* it does offer you a certain strange geeky peace.
* */

int app_setup_uring(struct submitter *s) {
	struct app_io_sq_ring *sring = &s->sq_ring;
	struct app_io_cq_ring *cring = &s->cq_ring;
	struct io_uring_params p;
	void *sq_ptr, *cq_ptr;

	/*
	* We need to pass in the io_uring_params structure to the io_uring_setup()
	* call zeroed out. We could set any flags if we need to, but for this
	* example, we don't.
	* */
	memset(&p, 0, sizeof(p));
	CHECK_NOT_M1(s->ring_fd = io_uring_setup(QUEUE_DEPTH, &p));

	/*
	* io_uring communication happens via 2 shared kernel-user space ring buffers,
	* which can be jointly mapped with a single mmap() call in recent kernels.
	* While the completion queue is directly manipulated, the submission queue
	* has an indirection array in between. We map that in as well.
	* */

	int sring_sz = p.sq_off.array + p.sq_entries * sizeof(unsigned);
	int cring_sz = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);

	/* In kernel version 5.4 and above, it is possible to map the submission and
	* completion buffers with a single mmap() call. Rather than check for kernel
	* versions, the recommended way is to just check the features field of the
	* io_uring_params structure, which is a bit mask. If the
	* IORING_FEAT_SINGLE_MMAP is set, then we can do away with the second mmap()
	* call to map the completion ring.
	* */
	if (p.features & IORING_FEAT_SINGLE_MMAP) {
		if (cring_sz > sring_sz) {
			sring_sz = cring_sz;
		}
		cring_sz = sring_sz;
	}

	/* Map in the submission and completion queue ring buffers.
	* Older kernels only map in the submission queue, though.
	* */
	CHECK_NOT_VOIDP(sq_ptr = mmap(0, sring_sz, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_POPULATE,
			s->ring_fd, IORING_OFF_SQ_RING), MAP_FAILED);

	if (p.features & IORING_FEAT_SINGLE_MMAP) {
		cq_ptr = sq_ptr;
	} else {
		/* Map in the completion queue ring buffer in older kernels separately */
		CHECK_NOT_VOIDP(cq_ptr = mmap(0, cring_sz, PROT_READ | PROT_WRITE,
				MAP_SHARED | MAP_POPULATE,
				s->ring_fd, IORING_OFF_CQ_RING), MAP_FAILED);
	}
	/* Save useful fields in a global app_io_sq_ring struct for later
	* easy reference */
	sring->head = (unsigned int*)sq_ptr + p.sq_off.head;
	sring->tail = (unsigned int*)sq_ptr + p.sq_off.tail;
	sring->ring_mask = (unsigned int*)sq_ptr + p.sq_off.ring_mask;
	sring->ring_entries = (unsigned int*)sq_ptr + p.sq_off.ring_entries;
	sring->flags = (unsigned int*)sq_ptr + p.sq_off.flags;
	sring->array = (unsigned int*)sq_ptr + p.sq_off.array;

	/* Map in the submission queue entries array */
	CHECK_NOT_VOIDP(s->sqes = (io_uring_sqe*)mmap(0, p.sq_entries * sizeof(struct io_uring_sqe),
			PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
			s->ring_fd, IORING_OFF_SQES), MAP_FAILED);

	/* Save useful fields in a global app_io_cq_ring struct for later
	* easy reference */
	cring->head = (unsigned int*)cq_ptr + p.cq_off.head;
	cring->tail = (unsigned int*)cq_ptr + p.cq_off.tail;
	cring->ring_mask = (unsigned int*)cq_ptr + p.cq_off.ring_mask;
	cring->ring_entries = (unsigned int*)cq_ptr + p.cq_off.ring_entries;
	cring->cqes = (io_uring_cqe*)cq_ptr + p.cq_off.cqes;

	return 0;
}

/*
* Output a string of characters of len length to stdout.
* We use buffered output here to be efficient,
* since we need to output character-by-character.
* */
void output_to_console(char *buf, int len) {
	while (len--) {
		fputc(*buf++, stdout);
	}
}

/*
* Read from completion queue.
* In this function, we read completion events from the completion queue, get
* the data buffer that will have the file data and print it to the console.
* */

void read_from_cq(struct submitter *s) {
	struct file_info *fi;
	struct app_io_cq_ring *cring = &s->cq_ring;
	struct io_uring_cqe *cqe;
	unsigned head;

	head = *cring->head;

	do {
		read_barrier();
		/*
		* Remember, this is a ring buffer. If head == tail, it means that the
		* buffer is empty.
		* */
		if (head == *cring->tail)
			break;

		/* Get the entry */
		cqe = &cring->cqes[head & *s->cq_ring.ring_mask];
		fi = (struct file_info*) cqe->user_data;
		if (cqe->res < 0)
			fprintf(stderr, "Error: %s\n", strerror(abs(cqe->res)));

		int blocks = (int) fi->file_sz / BLOCK_SZ;
		if (fi->file_sz % BLOCK_SZ) blocks++;

		for(int i = 0; i < blocks; i++)
			output_to_console((char*)(fi->iovecs[i].iov_base), fi->iovecs[i].iov_len);

		head++;
	} while (1);

	*cring->head = head;
	write_barrier();
}
/*
* Submit to submission queue.
* In this function, we submit requests to the submission queue. You can submit
* many types of requests. Ours is going to be the readv() request, which we
* specify via IORING_OP_READV.
*
* */
int submit_to_sq(char *file_path, struct submitter *s) {
	struct file_info *fi;

	int file_fd;
	CHECK_NOT_M1(file_fd=open(file_path, O_RDONLY));

	struct app_io_sq_ring *sring = &s->sq_ring;
	unsigned index = 0, current_block = 0, tail = 0, next_tail = 0;

	off_t file_sz = get_file_size(file_fd);
	if (file_sz < 0)
		return 1;
	off_t bytes_remaining = file_sz;
	int blocks = (int) file_sz / BLOCK_SZ;
	if (file_sz % BLOCK_SZ) blocks++;

	fi = (file_info*)malloc(sizeof(*fi) + sizeof(struct iovec) * blocks);
	if (!fi) {
		fprintf(stderr, "Unable to allocate memory\n");
		return 1;
	}
	fi->file_sz = file_sz;

	/*
	* For each block of the file we need to read, we allocate an iovec struct
	* which is indexed into the iovecs array. This array is passed in as part
	* of the submission. If you don't understand this, then you need to look
	* up how the readv() and writev() system calls work.
	* */
	while (bytes_remaining) {
		off_t bytes_to_read = bytes_remaining;
		if (bytes_to_read > BLOCK_SZ)
			bytes_to_read = BLOCK_SZ;

		fi->iovecs[current_block].iov_len = bytes_to_read;

		void *buf;
		CHECK_ZERO(posix_memalign(&buf, BLOCK_SZ, BLOCK_SZ));
		fi->iovecs[current_block].iov_base = buf;

		current_block++;
		bytes_remaining -= bytes_to_read;
	}

	/* Add our submission queue entry to the tail of the SQE ring buffer */
	next_tail = tail = *sring->tail;
	next_tail++;
	read_barrier();
	index = tail & *s->sq_ring.ring_mask;
	struct io_uring_sqe *sqe = &s->sqes[index];
	sqe->fd = file_fd;
	sqe->flags = 0;
	sqe->opcode = IORING_OP_READV;
	sqe->addr = (unsigned long) fi->iovecs;
	sqe->len = blocks;
	sqe->off = 0;
	sqe->user_data = (unsigned long long) fi;
	sring->array[index] = index;
	tail = next_tail;

	/* Update the tail so the kernel can see it. */
	if(*sring->tail != tail) {
		*sring->tail = tail;
		write_barrier();
	}

	/*
	* Tell the kernel we have submitted events with the io_uring_enter() system
	* call. We also pass in the IOURING_ENTER_GETEVENTS flag which causes the
	* io_uring_enter() call to wait until min_complete events (the 3rd param)
	* complete.
	* */
	int ret;
	CHECK_NOT_M1(ret = io_uring_enter(s->ring_fd, 1,1,
			IORING_ENTER_GETEVENTS));

	return 0;
}

int main(int argc, char *argv[]) {
	struct submitter *s;

	if (argc < 2) {
		fprintf(stderr, "%s: usage: %s <filename>\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	CHECK_NOT_NULL(s = (submitter*)malloc(sizeof(*s)));
	memset(s, 0, sizeof(*s));

	if(app_setup_uring(s)) {
		fprintf(stderr, "Unable to setup uring!\n");
		return EXIT_FAILURE;
	}

	for(int i = 1; i < argc; i++) {
		if(submit_to_sq(argv[i], s)) {
			fprintf(stderr, "Error reading file\n");
			return EXIT_FAILURE;
		}
		read_from_cq(s);
	}

	return EXIT_SUCCESS;
}
