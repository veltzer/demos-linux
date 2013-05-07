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
#include <us_helper.h>	// for CHECK_ASSERT()

/*
 * This is an example of using the 'static_assert' compile time assert
 * feature. It is only available in C++, not in C and only with the c11 standard
 * which is enabled in the flags.
 * For static asserts in the C language see a similar demo in the C section...
 * Also note that boost has a static assert feature (C++ only).
 * Note that the C style static asserts do not work here in C++ (see below...).
 *
 * required for the static_assert below to work...
 * EXTRA_COMPILE_FLAGS=-std=c++0x
 */

typedef struct _s1 {
	char c1;
	int i1;
	char c2;
} s1;
static_assert(sizeof(s1)==12,"something went wrong");
// this will cause a compile time error
//_Static_assert(sizeof(s1)==12,"something went wrong");

int main(int argc, char** argv, char** envp) {
	return EXIT_SUCCESS;
}
