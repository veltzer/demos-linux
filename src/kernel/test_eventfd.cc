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
#include <sys/eventfd.h> // for eventfd(2)
#include <stdio.h> // for printf(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for fork(2), close(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This tests eventfd code in the kernel...
*
* TODO:
* - call ioctl on the eventfd and get the signal...
*/
int main(int argc,char** argv,char** envp) {
	// file to be used
	const char *filename="/dev/mod_eventfd";
	// file descriptor
	int fd;

	CHECK_NOT_M1(fd=open(filename, O_RDWR));
	int efd;
	CHECK_NOT_M1(efd=eventfd(0, 0));
	int pid;
	CHECK_NOT_M1(pid=fork());
	if(pid==0) {
		printf("Child\n");
	} else {
		printf("Parent\n");
	}
	CHECK_NOT_M1(close(fd));
	return(0);
}
