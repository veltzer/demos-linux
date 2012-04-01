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

#include<strings.h> // for bzero(3)
#include<alloca.h> // for alloca(3)
#include<stdio.h> // for printf(3)
#include<sys/time.h> // for setrlimit(2)
#include<sys/resource.h> // for setrlimit(2)

#include<us_helper.h>

/*
 * This example shows how to allocate space on the stack using the alloca(3) function call.
 * This function is actually inlined assembly that never fails and only increments the stack
 * pointer and so is VERY VERY VERY fast. On the other hand it has it's own issues.
 *
 * On a standard linux machine this test will fail when reaching about 10M of ram. If you want
 * more then you can use setrlimit with the stack size you want to get more or use ulimit(1)
 * on the command line before running your program.
 *
 * Notice that once you set a limit using ulimit(1) on the command line then using this program
 * would fail since the limit was already set and so cannot be made higher (ulimit security).
 *
 * Notes:
 * - even if you did not zero the memory you got from alloca you would still get a stack
 * violation exception since in ubuntu 6.10 and onwards a stack protector is
 * automatically enabled for any thread calling alloca.
 */

void my_func(size_t size) {
	char* p=(char*)alloca(size);
	p[0]=0;
	//bzero(p,size);
}

int main(int argc,char** argv,char** envp) {
	unsigned int stack_size_to_set=50*1024*1024;
	struct rlimit rlim;
	rlim.rlim_max=stack_size_to_set;
	rlim.rlim_cur=stack_size_to_set;
	CHECK_NOT_M1(setrlimit(RLIMIT_STACK,&rlim));
	const size_t size=1024*1024;
	for(unsigned int i=0;i<100;i++) {
		printf("trying to allocate %d\n",i*size);
		my_func(size*i);
	}
	return EXIT_SUCCESS;
}
