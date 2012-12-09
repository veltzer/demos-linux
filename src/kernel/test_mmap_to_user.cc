/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <us_helper.h> // for CHECK_NOT_M1()
#include "shared.h"

/*
* This is a test for kernel driven mmap.
*
* The test:
* - opens the file.
* - asks (via ioctl) to mmap to it's space.
* - closes the file.
*/

//const bool do_single=true;
//const bool do_stress=false;
const bool do_single=false;
const bool do_stress=true;
const unsigned int count=10000;

int main(int argc,char** argv,char** envp) {
	// file to be used
	const char *filename="/dev/demo";
	// file descriptor
	int d=-1;
	// user space pointer
	void *ptr=NULL;

	//klog_clear();

	CHECK_NOT_M1(d=open(filename, O_RDWR));
	//printproc("demo");
	//klog_show_clear();
	waitkey(NULL);

	if (do_single) {
		// the size of data that we need
		const unsigned int size=1000000;
		CHECK_NOT_VOIDP(ptr=(void *)ioctl(d, IOCTL_DEMO_MAP, size), MAP_FAILED);
		//printproc("demo");
		//klog_show_clear();
		waitkey(NULL);

		INFO("trying to write on the buffer");
		memset(ptr, 'a', size);
		// get buffer from user space
		CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_READ, 'a'));
		CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_WRITE, 'a' + 1));
		memcheck(ptr, 'a' + 1, size);

		CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_UNMAP, NULL));
		//printproc("demo");
		//klog_show_clear();
		waitkey(NULL);
	}
	if (do_stress) {
		do_prog_init();
		for (unsigned int i=0; i < count; i++) {
			do_prog(i, 10, count);
			unsigned int size=(i % 1000 + 1) * 4096;
			char c=i % 20 + 'a';
			CHECK_NOT_NULL(ptr=(void *)ioctl(d, IOCTL_DEMO_MAP, size));
			// set the buffer to c
			memset(ptr, c, size);
			// get buffer from user space
			CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_READ, c));
			// ask the kernel to write c+1
			CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_WRITE, c + 1));
			// check that the buffer has the right data
			memcheck(ptr, c + 1, size);
			// get ridd of the in kernel buffer
			CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_UNMAP, NULL));
		}
		do_prog_finish();
	}

	CHECK_NOT_M1(close(d));
	//printproc("demo");
	//klog_show_clear();
	waitkey(NULL);

	return(0);
}
