/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2025 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for EXIT_SUCCESS, malloc(3), free(3)
#include <stdio.h>	// for printf(3)
#include <err_utils.h>	// for CHECK_POSITIVE(), CHECK_NOT_NULL(), CHECK_IN_RANGE()
#include <unistd.h>	// for confstr(3)

/*
 * This example shows how to get the version of glibc
 */

int main() {
	size_t n=CHECK_POSITIVE(confstr(_CS_GNU_LIBC_VERSION, NULL, (size_t)0));
	char* pathbuf=(char*)CHECK_NOT_NULL(malloc(n));
	CHECK_IN_RANGE(confstr(_CS_GNU_LIBC_VERSION, pathbuf, n), 1, n+1);
	printf("confstr(_CS_GNU_LIBC_VERSION) returned %s\n", pathbuf);
	free(pathbuf);
	return EXIT_SUCCESS;
}
