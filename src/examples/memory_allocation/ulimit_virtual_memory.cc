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

#include<stdio.h> // for printf(3), perror(3)
#include<stdlib.h> // for exit(3), malloc(3), EXIT_SUCCESS, EXIT_FAILURE
#include<sys/time.h> // for setrlimit(2)
#include<sys/resource.h> // for setrlimit(2)

#include<us_helper.h>

/*
 * This example explores the use of limits. You can limit the size of your programs RAM
 * and crash long before you get into swapping (recommended).
 * Many more limits exist. Try to use as much of them as possible.
 * set setrlimit(2) for more details.
 *
 * You can ofcourse skip the setting of ulimit in the source code and use ulimit(1) on the
 * command line instead.
 */

int main(int argc,char** argv,char** envp) {
	const int max_megs=50;
	struct rlimit rlim;
	rlim.rlim_max=max_megs*1024*1024;
	rlim.rlim_cur=max_megs*1024*1024;
	CHECK_NOT_M1(setrlimit(RLIMIT_AS,&rlim));
	int d=0;
	while(true) {
		void* m=malloc(1024*1024);
		if(m==NULL) {
			printf("Oh oh, no memory for me\n");
			exit(EXIT_FAILURE);
		}
		printf("managed to allocate %d megs\n",d);
		d++;
	}
	return EXIT_SUCCESS;
}
