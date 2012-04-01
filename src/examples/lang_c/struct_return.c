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
 * This is an attempt to return a struct from a function
 */

typedef struct _foo {
	int a;
	int b;
} foo;

foo giveMeFoo(void) {
	// following line causes a compilation error...
	//return {3,4};
	foo f;
	return(f);
}

int main(int argc,char** argv,char** envp) {
	foo f=giveMeFoo();
	printf("f.a is %d\n",f.a);
	printf("f.b is %d\n",f.b);
	return EXIT_SUCCESS;
}
