/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example of initializing a struct in C++...
 */

struct mystruct {
	int a;
	int b;
};

int main(int argc, char** argv, char** envp) {
	// this is C99 style. This does not work in g++ by default although it
	//does work in gcc.
	/*
	   struct mystruct v1={ .a=6, .b=7 };
	   printf("v1.a is %d\n",v1.a);
	   printf("v1.b is %d\n",v1.b);
	 */
	// this is the GNU style...
	struct mystruct v2={ a : 8, b : 9 };
	printf("v2.a is %d\n", v2.a);
	printf("v2.b is %d\n", v2.b);
	// this will always work
	struct mystruct v3;
	printf("v3.a is %d\n", v3.a);
	printf("v3.b is %d\n", v3.b);
	v3.a=10;
	v3.b=11;
	printf("v3.a is %d\n", v3.a);
	printf("v3.b is %d\n", v3.b);
	// the C++ style...
	/*
	   struct mystruct v4():a(12),b(13);
	   printf("v4.a is %d\n",v4.a);
	   printf("v4.b is %d\n",v4.b);
	 */
	return EXIT_SUCCESS;
}
