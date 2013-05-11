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
 * An example showing how to use the preprocessor in order to avoid
 * repetative code.
 */

#define make_add(t) t add ## t(t a, t b) { return a+b; }

make_add(float);
make_add(double);

/*
 * float addfloat(float a,float b) {
 *	return a+b;
 * }
 * double adddouble(double a,double b) {
 *	return a+b;
 * }
 */

int main(int argc, char** argv, char** envp) {
	float x=4.5f;
	float y=3.2f;
	printf("%f+%f=%f\n", x, y, addfloat(x, y));
	return EXIT_SUCCESS;
}
