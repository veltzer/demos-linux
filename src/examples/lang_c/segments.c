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
 * This example shows the pointer values in various segments...
 */

int var_data=4;
const char* buf_data="this is the data buffer";

int main(int argc, char** argv, char** envp) {
	int var_stack=5;
	const char* buf_stack="this is a buffer";
	printf("var_stack is %p\n", &var_stack);
	printf("buf_stack is %p\n", buf_stack);
	printf("var_data is %p\n", &var_data);
	printf("buf_data is %p\n", buf_data);
	return EXIT_SUCCESS;
}
