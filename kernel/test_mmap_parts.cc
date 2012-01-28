#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "us_helper.hh"

/*
 *      This test shows how you can do mmap via an ioctl...
 */

bool do_mmap_once = false;
bool do_ioctl_once = false;
bool do_ioctl_once_write = false;
bool do_stress_ioctl = false;
bool do_stress_mmap = true;

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
	// size of the buffer we want
	const int size = 1024 * 1024;
	// file handle
	int d;
	// result integer
	int res;
	// file to be used
	const char *filename = "/dev/demo";

	sc(d = open(filename, O_RDWR));

	//printf("setting the size to %d\n",size);
	//sc(res=ioctl(d,6,size));

	if (do_mmap_once) {
		void *p;
		klog_clear();
		SCPE(p = mmap(
		             NULL,                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* we DO NOT recommend an address - better to let the kernel decide */
		             size,                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* the size we need */
		             PROT_READ | PROT_WRITE,                                                                                                                                                                                                                                                                                                                                                                                                                                        /* we want read AND write */
		             MAP_SHARED | MAP_POPULATE,                                                                                                                                                                                                                                                                                                                                                                                                                                     /* we want to shard with kernel and don't want page faults */
		             d,                                                                                                                                                                                                                                                                                                                                                                                                                                                             /* file descriptor */
		             0                                                                                                                                                                                                                                                                                                                                                                                                                                                              /* offset */
		             ), "mmap");
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		klog_clear();
		sc(munmap(p, size));
		klog_show();
		printbuddy();
	}
	if (do_ioctl_once) {
		printproc("demo");
		klog_clear();
		// trying to map memory
		sc(res = ioctl(d, 4, NULL));
		void *p = (void *)res;
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		klog_clear();
		// trying to unmap memory
		sc(res = ioctl(d, 5, NULL));
		klog_show();
		printproc("demo");
	}
	if (do_ioctl_once_write) {
		printproc("demo");
		klog_clear();
		// trying to map memory
		sc(res = ioctl(d, 4, NULL));
		void *p = (void *)res;
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		memset(p, 0, size);
		klog_clear();
		// trying to unmap memory
		sc(res = ioctl(d, 5, NULL));
		klog_show();
		printproc("demo");
	}
	if (do_stress_ioctl) {
		const int number = 10;
		for (int i = 0; i < number; i++) {
			// trying to map memory
			sc(res = ioctl(d, 4, NULL));
			void *p = (void *)res;
			printf("the pointer I got is %p\n", p);
			// trying to unmap memory
			sc(res = ioctl(d, 5, NULL));
		}
	}
	if (do_stress_mmap) {
		const int number = 100000;
		for (int i = 0; i < number; i++) {
			void *p;
			SCPE(p = mmap(
			             NULL,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   /* we DO NOT recommend an address - better to let the kernel decide */
			             size,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   /* the size we need */
			             PROT_READ | PROT_WRITE,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /* we want read AND write */
			             MAP_SHARED | MAP_POPULATE,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              /* we want to shard with kernel and don't want page faults */
			             d,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      /* file descriptor */
			             0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       /* offset */
			             ), "mmap");
			printf("the pointer I got is %p\n", p);
			memset(p, 0, size);
			sc(munmap(p, size));
			printbuddy();
		}
	}
	sc(close(d));
	return(0);
}
