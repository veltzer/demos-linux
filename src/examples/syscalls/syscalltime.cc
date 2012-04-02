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

#include<firstinclude.h>
#include<stdio.h> // for printf(3)
#include<unistd.h> // for syscall(2), __NR_getpid
#include<sys/syscall.h> // for syscall(2)
#include<sys/time.h> // for gettimeofday(2)
#include<us_helper.h> // for micro_diff()

/*
* This demo times how long it takes to call a syscall.
* It is a simple loop surrounded by gettimeofday.
*
* How long is it?
* About 1/3 mic on a fast computer.
* About 1.2 mic on my laptop.
* Probably more on ARM processors.
*/

int main(int argc,char** argv,char** envp) {
	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("doing %d syscalls\n",loop);
	gettimeofday(&t1,NULL);
	for(unsigned int i=0;i<loop;i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
		//syscall(__NR_getpid);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return EXIT_SUCCESS;
}
