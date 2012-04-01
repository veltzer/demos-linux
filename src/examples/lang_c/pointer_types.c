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

#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This demos how to use compiler attributes to make sure we don't mix pointers that
 * refer to different memory spaces...
 *
 * Regular software developers usually do not use these features...
 */

// this is taken from the kernel sources (include/linux/compiler.h)
// currently this does not seem to work

//#define DO_IT

#ifdef DO_IT
#define __space1 __attribute__((noderef, address_space(1)))
#define __space2 __attribute__((noderef, address_space(2)))
#else // DO_IT
#define __space1
#define __space2
#endif // DO_IT
void func1(void *__space1 pointer) {
}

void *__space1 malloc1(unsigned int size) {
	return(NULL);
}

void func2(void *__space2 pointer) {
}

void *__space2 malloc2(unsigned int size) {
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	return EXIT_SUCCESS;
}
