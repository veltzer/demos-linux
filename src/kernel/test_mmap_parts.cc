/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
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
#include <us_helper.h> // for CHECK_NOT_M1(), CHECK_NOT_VOIDP()

/*
* This test shows how you can do mmap via an ioctl...
*/

bool do_mmap_once=false;
bool do_ioctl_once=false;
bool do_ioctl_once_write=false;
bool do_stress_ioctl=false;
bool do_stress_mmap=true;

void print_data(void *data, int size) {
	int msize;

	if (size < 256) {
		msize=size;
	} else {
		msize=256;
	}
	char *pdata=(char *)malloc(msize);
	strncpy(pdata, (char *)data, msize - 1);
	pdata[msize - 1]='\0';
	fprintf(stderr, "data is [%s]\n", pdata);
	free(pdata);
}

int main(int argc,char** argv,char** envp) {
	// size of the buffer we want
	const int size=1024 * 1024;
	// result integer
	int res;
	// file to be used
	const char *filename="/dev/demo";

	int d=CHECK_NOT_M1(open(filename, O_RDWR));

	//printf("setting the size to %d\n",size);
	//res=CHECK_NOT_M1(ioctl(d,6,size));

	if (do_mmap_once) {
		klog_clear();
		void *p=CHECK_NOT_VOIDP(mmap(
			NULL, /* we DO NOT recommend an address - better to let the kernel decide */
			size, /* the size we need */
			PROT_READ | PROT_WRITE, /* we want read AND write */
			MAP_SHARED | MAP_POPULATE, /* we want to shard with kernel and don't want page faults */
			d, /* file descriptor */
			0 /* offset */
		), MAP_FAILED);
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		klog_clear();
		CHECK_NOT_M1(munmap(p, size));
		klog_show();
		printbuddy();
	}
	if (do_ioctl_once) {
		printproc("demo");
		klog_clear();
		// trying to map memory
		res=CHECK_NOT_M1(ioctl(d, 4, NULL));
		void *p=(void *)res;
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		klog_clear();
		// trying to unmap memory
		res=CHECK_NOT_M1(ioctl(d, 5, NULL));
		klog_show();
		printproc("demo");
	}
	if (do_ioctl_once_write) {
		printproc("demo");
		klog_clear();
		// trying to map memory
		res=CHECK_NOT_M1(ioctl(d, 4, NULL));
		void *p=(void *)res;
		printf("the pointer I got is %p\n", p);
		klog_show();
		printproc("demo");
		memset(p, 0, size);
		klog_clear();
		// trying to unmap memory
		res=CHECK_NOT_M1(ioctl(d, 5, NULL));
		klog_show();
		printproc("demo");
	}
	if (do_stress_ioctl) {
		const int number=10;
		for (int i=0; i < number; i++) {
			// trying to map memory
			res=CHECK_NOT_M1(ioctl(d, 4, NULL));
			void *p=(void *)res;
			printf("the pointer I got is %p\n", p);
			// trying to unmap memory
			res=CHECK_NOT_M1(ioctl(d, 5, NULL));
		}
	}
	if (do_stress_mmap) {
		const int number=100000;
		for (int i=0; i < number; i++) {
			void *p=CHECK_NOT_VOIDP(mmap(
				NULL, /* we DO NOT recommend an address - better to let the kernel decide */
				size, /* the size we need */
				PROT_READ | PROT_WRITE, /* we want read AND write */
				MAP_SHARED | MAP_POPULATE, /* we want to shard with kernel and don't want page faults */
				d, /* file descriptor */
				0 /* offset */
			), MAP_FAILED);
			printf("the pointer I got is %p\n", p);
			memset(p, 0, size);
			CHECK_NOT_M1(munmap(p, size));
			printbuddy();
		}
	}
	CHECK_NOT_M1(close(d));
	return(0);
}
