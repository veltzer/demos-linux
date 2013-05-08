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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, rand(3)

/*
 * This example shows how to treat constant vs non constant arguments
 * differently...
 *
 * TODO:
 * - stop the compile on constants or non constants...
 *
 * References:
 * http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Other-Builtins.html
 */

inline void do_something_with_constant1(int a) __attribute__((always_inline));
inline void do_something_with_constant1(int a) {
	if(!__builtin_constant_p(a)) {
		printf("bad boy!\n");
	} else {
		printf("You are a good boy passing only constants... (%d)\n",a);
	}
}

#define ct_assert(e) (sizeof(char[1 - 2*!(e)]))
#define do_something_with_constant(a) \
	( __builtin_constant_p(a) ? 7 : ct_assert(0==1) )

int main(int argc, char** argv, char** envp) {
	// this will do something bad at runtime
	int x=rand();
	do_something_with_constant1(x);
	// this one will not
	do_something_with_constant1(13);
	return EXIT_SUCCESS;
}
