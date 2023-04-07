/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2023 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for sysconf(3), getpid(2)
#include <multiproc_utils.h>	// for my_system()
#include <signal_utils.h>	// for signal_register_handler_signal()
#include <err_utils.h>	// for CHECK_NOT_M1()

/*
 * This example shows that the data segment is organized into pages. It shows
 * that if you step over data then you will only get seg faulted once you step
 * over the page boundary.
 *
 * EXTRA_COMPILE_FLAGS=-Wno-array-bounds
 */

// a must be last...
int a=1;
// this is data segment data...
char* ptr;
unsigned int page_size;
sighandler_t old_handler;

/*
 * This is a signal handler to handle the segmentation faults we will generate...
 */
void segv_handler(int sig) {
	// printf("in segv handler\n");
	printf("in segv handler: ptr=%p, offset is %ld\n", ptr, long(ptr)%page_size);
	// call the old handler...
	old_handler(sig);
}

int main(int argc, char** argv, char** envp) {
	page_size=CHECK_NOT_M1(sysconf(_SC_PAGESIZE));
	printf("page size is %d\n", page_size);
	my_system("pmap %d", getpid());
	printf("address of a is %p, look it up above...\n", &a);
	printf("address of ptr is %p, look it up above...\n", &ptr);
	printf("address of page_size is %p, look it up above...\n", &page_size);
	printf("address of old_handler is %p, look it up above...\n", &old_handler);
	waitkey(NULL);
	// lets install our own SIGSEGV signal handler so that we could print the address we
	// failed at...
	old_handler=signal_register_handler_signal(SIGSEGV, segv_handler);
	ptr=(char*)&a;
	// the next line is really needed. If I leave it out then the seg fault happens much
	// sooner since I'm stepping over stuff which is needed by the libc library for either
	// catching the signal or doing printf. I know it is weird that libc puts stuff in
	// my data segment but that is the case. The offset has to be large enough no to
	// step over libc.
	ptr+=200;
	for(unsigned int i=0; i<page_size*2; i++) {
		// printf("still alive: ptr=%p, i=%d, offset is %d\n",ptr,i,int(ptr)%page_size);
		*ptr=0;
		// this is not strictly necessary...
		// __sync_synchronize();
		ptr++;
	}
	// do not remove the next call (see above note)
	printf("this is a dummy print which is never reached and must stay here so that the compiler will not optimize away the loop before when compiling with optimization turned on");
	return EXIT_SUCCESS;
}
