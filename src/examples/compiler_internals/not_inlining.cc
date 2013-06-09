/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example showing how to force the compiler not to inline the function.
 * How can you check that this works? compare the two disassembled version
 * resulting from putting the "noinline" line into a comment
 */

int doit(int a, int b) __attribute__((noinline));
int doit(int a, int b) {
	return a+b;
}

int main(int argc, char** argv, char** envp) {
	printf("did you know that 2+2=%d\n", doit(2, 2));
	return EXIT_SUCCESS;
}
