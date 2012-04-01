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

#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is an example showing how to force the compiler not to inline the function.
 * How can you check that this works? compare the two disassembled version
 * resulting from putting the "noinline" line into a comment
 */

int doit(int a,int b) __attribute__((noinline));
int doit(int a,int b) {
	return a+b;
}

int main(int argc,char** argv,char** envp) {
	printf("did you know that 2+2=%d\n",doit(2,2));
	return EXIT_SUCCESS;
}
