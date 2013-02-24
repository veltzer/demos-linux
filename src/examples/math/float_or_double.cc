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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example for all the folk wishing to do numerical applications and
 *want to switch
 * between float and double at compile time instead of at run time.
 */

// first option - using floats...
// #define USING_FLOATS
// typedef float some_float;
// #define MY_PRINTF_ARG "%f"

// second option - using doubles...
#define USING_DOUBLES
typedef double some_float;
#define MY_PRINTF_ARG "%lf"

// MC - a macro to make constants
#ifdef USING_FLOATS
#define MC(X) X ## f
#endif
#ifdef USING_DOUBLES
#define MC(X) X
#endif

/*
 * example of a type neutral function
 */
inline some_float add(some_float a, some_float b) {
	return a+b;
}

int main(int argc, char** argv, char** envp) {
	some_float f1=MC(3.14);
	some_float f2=MC(3.14);
	printf(MY_PRINTF_ARG "+" MY_PRINTF_ARG "=" MY_PRINTF_ARG "\n", f1, f2, add(f1, f2));
	return EXIT_SUCCESS;
}
