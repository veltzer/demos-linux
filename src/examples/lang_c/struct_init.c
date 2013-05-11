/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example of initializing a struct in various ways in C...
 */

struct mystruct {
	int a;
	int b;
	int c;
};

int main(int argc, char** argv, char** envp) {
	// this is C99 style. It doesn't require any specific flags to gcc these days
	// fields not initialized will be 0
	struct mystruct v1={ .a=6, .b=7 };
	printf("v1.a is %d\n", v1.a);
	printf("v1.b is %d\n", v1.b);
	printf("v1.c is %d\n", v1.c);
	// this is the GNU style...
	// fields not initialized will be 0
	struct mystruct v2={ a: 8, b: 9 };
	printf("v2.a is %d\n", v2.a);
	printf("v2.b is %d\n", v2.b);
	printf("v2.c is %d\n", v2.c);
	// this will always work (values will be junk)
	struct mystruct v3;
	printf("v3.a is %d\n", v3.a);
	printf("v3.b is %d\n", v3.b);
	printf("v3.c is %d\n", v3.c);
	// this will also always work but because we are accessing the values later
	// then gcc will 0 them for us...
	struct mystruct v4;
	printf("v4.a is %d\n", v4.a);
	printf("v4.b is %d\n", v4.b);
	printf("v4.c is %d\n", v4.c);
	v4.a=10;
	v4.b=11;
	v4.c=12;
	printf("v4.a is %d\n", v4.a);
	printf("v4.b is %d\n", v4.b);
	printf("v4.c is %d\n", v4.c);
	return EXIT_SUCCESS;
}
