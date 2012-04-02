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
#include<stdlib.h> // for EXIT_SUCCESS

/*
* This demo shows how to print an unsigned long in a big hexa format...
*/

int main(int argc,char** argv,char** envp) {
	unsigned long i = 0xabcd0123;
	void* p = (void *)i;
	char* cp=(char*)i;

	printf("void* printed as '%%p' is %p\n", p);
	printf("char* (and any other pointer) printed as '%%p' is %p\n", cp);
	printf("'%%lx' is %lx\n", i);
	printf("'%%lX' is %lX\n", i);
	printf("'%%#lx' is %#lx\n", i);
	printf("'%%#lX' is %#lX\n", i);
	printf("with conversion '%%lX' is %lX\n", (unsigned long)p);
	int width=5;
	int num=4;
	printf("printing with non compile time width %*d\n", width, num);
	return EXIT_SUCCESS;
}
