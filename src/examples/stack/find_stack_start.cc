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
#include <pthread.h>	// for pthread_self(3), pthread_attr_t(S), pthread_getattr_np(3), pthread_attr_getstack(3)
#include <err_utils.h> // for CHECK_ZERO_ERRNO()

/*
	This program tries to find the start address of your stack

	Why would you want this?
	- hacking. Hackers love to find the start of the stack so they can know
	where do they want to mess things up or maybe call mprotect(2) on.
	- you want to see if you are in an infinite recursion.

	NOTES:
	- to turn off kernel randomization of address space layout:
	echo 0 > /proc/sys/kernel/randomize_va_space

	TODO:
	try to do thing via pthreads.

	EXTRA_LINK_FLAGS=-lpthread
*/

int main(int argc, char** argv, char** envp) {
	printf("getstackpointer() returned [%p]\n", getstackpointer());
	pthread_attr_t gattr;
	CHECK_ZERO_ERRNO(pthread_getattr_np(pthread_self(), &gattr));
	size_t v;
	void* stkaddr;
	CHECK_ZERO_ERRNO(pthread_attr_getstack(&gattr, &stkaddr, &v));
	void* endaddr=(void*)((char*)stkaddr+v+2*getpagesize());
	printf("stack address from pthread is [%p]\n", stkaddr);
	printf("stack size from pthread is [0x%x] bytes\n", v);
	printf("stack end from pthread is [%p]\n", endaddr);
	pid_t mypid=getpid();
	my_system("cat /proc/%d/maps", mypid);
	//my_system("pmap %d", mypid);
	return EXIT_SUCCESS;
}
