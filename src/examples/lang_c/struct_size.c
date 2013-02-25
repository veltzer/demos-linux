/*
 *      This file is part of the linuxapi project.
 *      Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 *      The linuxapi package is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      The linuxapi package is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with the GNU C Library; if not, write to the Free
 *      Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *      02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This example explores the sizeof structures.
 *
 * This example shows that:
 * - the size of an empty structure is 0 in C (in C++ it's actually 1 to make it an object).
 * - just one character does NOT pad the size to 4 or something.
 */

typedef struct _empty {
} empty;

typedef struct _onechar {
	char mychar;
} onechar;

int main(int argc, char** argv, char** envp) {
	printf("sizeof(empty) is %d\n", sizeof(empty));
	printf("sizeof(onechar) is %d\n", sizeof(onechar));
	return EXIT_SUCCESS;
}
