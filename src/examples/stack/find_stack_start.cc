/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <lowlevel_utils.h> // for getstackpointer()
#include <multiproc_utils.h>	// for my_system()
#include <unistd.h>	// for getpid(2), getpagesize(2)
#include <proc_utils.h> // for proc_get_start_stack(), proc_print_mmap_self()
#include <pthread_utils.h> // for pthread_getstack()

/*
	This program tries to find the start address of your stack

	Why would you want this?
	- hacking. Hackers love to find the start of the stack so they can know
	where do they want to mess things up or maybe call mprotect(2) on.
	- you want to see if you are in an infinite recursion.

	NOTES:
	- to turn off kernel randomization of address space layout:
	echo 0 > /proc/sys/kernel/randomize_va_space
	the ubuntu default for this /proc value is 2.

	TODO:
	try to do thing via pthreads.

	EXTRA_LINK_FLAGS=-lpthread -lprocps
*/

int main(int argc, char** argv, char** envp) {
	printf("getstackpointer() returned [%p]\n", getstackpointer());
	printf("proc_get_start_stack() returned [0x%lx]\n", proc_get_start_stack());
	void* stackaddr;
	size_t stacksize;
	pthread_getstack(&stackaddr, &stacksize);
	printf("pthread stack address is [%p]\n", stackaddr);
	printf("This is the correct one...\n");
	proc_print_mmap_self();
	return EXIT_SUCCESS;
}
