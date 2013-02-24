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
#include <stdlib.h>	// for EXIT_SUCCESS, malloc(3), free(3)
#include <stdio.h>	// for printf(3)
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <unistd.h>	// for confstr(3)

/*
 * This example extracts the path to POSIX.2 utils
 * This example is taken from 'man 3 confstr'
 */

int main(int argc, char** argv, char** envp) {
	size_t n=CHECK_POSITIVE(confstr(_CS_PATH, NULL, (size_t)0));
	char *pathbuf=(char*)CHECK_NOT_NULL(malloc(n));
	CHECK_IN_RANGE(confstr(_CS_PATH, pathbuf, n), 1, n+1);
	printf("confstr(_CS_PATH) returned %s\n", pathbuf);
	free(pathbuf);
	return EXIT_SUCCESS;
}
