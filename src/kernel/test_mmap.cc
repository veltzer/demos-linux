#include <stdio.h> // for fprintf(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <stdlib.h> // for free(3), malloc(3)
#include <string.h> // for strncpy(3)
#include <sys/mman.h> // for mmap(2), munmap(2)
#include <sys/ioctl.h> // for ioctl(2)

#include "shared.h" // for the ioctl numbers

#include "us_helper.hh"

/*
 *      This is a test for the mmap demo:
 *      0. open.
 *      1. allocate memory in kernel space and get pointer in used (mmap).
 *      2. ask kernel to manipulate the memory.
 *      3. print it out.
 *      4. unmap.
 *      5. close.
 */

bool do_play = false;
bool do_stress = false;
bool do_vma = true;

void print_data(void *data, int size) {
	int msize;

	if (size < 256) {
		msize = size;
	} else {
		msize = 256;
	}
	char *pdata = (char *)malloc(msize);
	strncpy(pdata, (char *)data, msize - 1);
	pdata[msize - 1] = '\0';
	fprintf(stderr, "data is [%s]\n", pdata);
	free(pdata);
}

int main(int argc, char **argv, char **envp) {
	// the data pointer
	void *data;
	// another data pointer
	void *data2;
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;
	// flags
	const int flags = MAP_PRIVATE | MAP_POPULATE;
	// the size of data that we need
	const int size = 1000000;
	// at what offset
	const int offset = 0;

	fprintf(stderr, "Asking the kernel to open the handle\n");
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	//printproc();

	CHECK_NOT_VAL(data = mmap(
	             NULL, /* we DO NOT recommend an address - better to let the kernel decide */
	             size, /* the size we need */
	             PROT_READ | PROT_WRITE, /* we want read AND write */
	             flags, /* we don't want page faults */
	             d, /* file descriptor */
	             offset /* offset */
	             ), MAP_FAILED);
	CHECK_NOT_VAL(data2 = mmap(
	             NULL, /* we DO NOT recommend an address - better to let the kernel decide */
	             size, /* the size we need */
	             PROT_READ | PROT_WRITE, /* we want read AND write */
	             flags, /* we don't want page faults */
	             d, /* file descriptor */
	             offset /* offset */
	             ), MAP_FAILED);
	fprintf(stderr, "pointer I got is %p\n", data);
	print_data(data, size);
	printproc("demo");

	fprintf(stderr, "Putting some data in the buffer...\n");
	memset(data, 'a', size);
	//strncpy((char*)data,"here is some data",size);
	print_data(data, size);

	if (do_play) {
		for (char i = 'a'; i < 'z'; i += 2) {
			fprintf(stderr, "Setting memory to ['%c']\n", i);
			memset(data, i, size);
			print_data(data, size);
			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_READ, NULL));
			print_data(data, size);
			waitkey(NULL);

			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_WRITE, i + 1));
			print_data(data, size);

			fprintf(stderr, "Asking kernel to read memory...\n");
			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_READ, NULL));
			print_data(data, size);
			waitkey(NULL);
		}
	}
	if (do_stress) {
		for (char i = 'a'; i < 'z'; i += 2) {
			memset(data, i, size);
			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_READ, NULL));
			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_WRITE, i + 1));
			CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_READ, NULL));
		}
	}
	if (do_vma) {
		CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_PRINT, data));
		void *p = (void *)((char *)data + size / 2);
		CHECK_NOT_M1(ioctl(d, IOCTL_MMAP_PRINT, p));
		waitkey(NULL);
	}

	CHECK_NOT_M1(munmap(data, size));
	printproc("demo");
	CHECK_NOT_M1(munmap(data2, size));
	printproc("demo");
	CHECK_NOT_M1(close(d));
	return(0);
}
