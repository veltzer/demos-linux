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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)

/*
 * This example shows how to pad a structure to cache line size
 */

const unsigned int cache_line_size=64;

typedef struct _barestruct {
	int field1;
	char foo;
} barestruct;

typedef struct _mystruct {
	int field1;
	char foo;
	char padding[cache_line_size-sizeof(barestruct)];
} mystruct;

/*
 * This does not work...
 * #include <stddef.h> // for offsetof(3)
 * typedef struct _mystruct2 {
 * int field1;
 * char foo;
 * char padding[64-__builtin_offsetof(_mystruct2,foo)];
 * } mystruct2;
 */

int main(int argc, char** argv, char** envp) {
	printf("sizeof(barestruct)=%zd\n", sizeof(barestruct));
	printf("sizeof(mystruct)=%zd\n", sizeof(mystruct));
	return EXIT_SUCCESS;
}
