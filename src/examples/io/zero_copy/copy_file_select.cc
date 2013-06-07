/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, malloc(3), free(3)
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for read(2), write(2), close(2), getpagesize()
#include <sys/select.h>	// for select(2), FD_ZERO(), FD_SET(), FD_ISSET()
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_NULL(), my_max()

/*
 * This example shows how to copy a copy file program using the select system
 * call.
 *
 * The idea is to have a buffer of a certain size. When there is room in the buffer
 * keep calling read. When there is data in the buffer keep calling write. Handle
 * the buffer in a circular way. No need for any synchronization in this implementation
 * since only one thread is doing all the work.
 *
 * The advantage of this implementation over a simple read and write implementation is that
 * requests for read(2) and write(2) could be issued by this app * at the same time *.
 * This is especially true for read(2) calls which are synchroneous and blocking. In the
 * simple read+write implementation a write will never be issued while a read is runnig
 * which in this implementation we could issue a write while a read is going on. This is
 * especially useful when you are copying files between two different hard disks.
 */

class Pipe {
private:
	char* buf;
	size_t size;
	size_t pos_read;
	size_t pos_write;

public:
	inline Pipe(const size_t isize) {
		size=isize;
		pos_read=0;
		pos_write=0;
		buf=(char*)CHECK_NOT_NULL(malloc(isize));
	}
	inline ~Pipe() {
		free(buf);
	}
	/* return the empty room of a pipe */
	inline size_t room() {
		if (pos_read <= pos_write)
			return size-pos_write+pos_read-1;
		else
			return pos_read-pos_write-1;
	}
	/* return the occupied room of a pipe */
	inline size_t data() {
		if (pos_read <= pos_write)
			return pos_write-pos_read;
		else
			return size-pos_read+pos_write;
	}
	inline bool haveData() {
		return data()>0;
	}
	inline bool haveRoom() {
		return room()>0;
	}
	inline bool canRead() {
		return haveRoom();
	}
	inline bool canWrite() {
		return haveData();
	}
	/* read data into the buffer (at pos_write) */
	inline bool doRead(int fd) {
		size_t count;
		if (pos_read <= pos_write) {
			count = size-pos_write;
		} else {
			count = pos_write-pos_read;
		}
		ssize_t len=CHECK_NOT_M1(read(fd, buf+pos_write, count));
		pos_write+=len;
		pos_write%=size;
		return len==0;
	}
	/* write data from the buffer (at pos_read) */
	inline void doWrite(int fd) {
		size_t count;
		if (pos_read <= pos_write) {
			count = pos_write-pos_read;
		} else {
			count = size-pos_read;
		}
		ssize_t len=CHECK_NOT_M1(write(fd, buf+pos_read, count));
		pos_read+=len;
		pos_read%=size;
	}
};

class Selector {
private:
	fd_set rfds;
	fd_set wfds;
	int fd_max;

public:
	inline Selector() {
	}
	inline void null() {
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		fd_max=0;
	}
	inline void addReadFd(int fd) {
		FD_SET(fd, &rfds);
		fd_max=my_max(fd_max, fd);
	}
	inline void addWriteFd(int fd) {
		FD_SET(fd, &wfds);
		fd_max=my_max(fd_max, fd);
	}
	inline bool isReadActive(int fd) {
		return FD_ISSET(fd, &rfds);
	}
	inline bool isWriteActive(int fd) {
		return FD_ISSET(fd, &wfds);
	}
	inline void doSelect() {
		CHECK_NOT_M1(select(fd_max+1, &rfds, &wfds, NULL, NULL));
	}
};

void copy_file(const char* filein, const char* fileout, const unsigned int bufsize) {
	Pipe p(bufsize);
	int fdin=CHECK_NOT_M1(open(filein, O_RDONLY|O_LARGEFILE, 0666));
	int fdout=CHECK_NOT_M1(open(fileout, O_WRONLY|O_CREAT|O_TRUNC|O_LARGEFILE, 0666));
	Selector s;
	bool eof=false;
	do {
		s.null();
		if(p.canRead() && !eof) {
			s.addReadFd(fdin);
		}
		if(p.canWrite()) {
			s.addWriteFd(fdout);
		}
		s.doSelect();
		// It doesn't matter who we handle first: if we handle the read first then
		// we will have more to write. If we handle the write first then we will
		// have more room to read into.
		if (s.isReadActive(fdin)) {
			if(p.doRead(fdin)) {
				eof=true;
			}
		}
		if (s.isWriteActive(fdout)) {
			p.doWrite(fdout);
		}
	} while (!eof || p.canWrite());
	CHECK_NOT_M1(close(fdin));
	CHECK_NOT_M1(close(fdout));
}

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [infile] [outfile] [num_pages]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const char* filein=argv[1];
	const char* fileout=argv[2];
	const unsigned int num_pages=atoi(argv[3]);
	copy_file(filein, fileout, num_pages*getpagesize());
	return EXIT_SUCCESS;
}
