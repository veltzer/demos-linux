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
#include <us_helper.h>	// for CHECK_NOT_ZERO()
#include <unistd.h>	// for confstr(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This demp shows how to get the version of the pthread library you are using.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

int main(int argc, char** argv, char** envp) {
	const unsigned int bufsize=256;
	char name[bufsize];
	CHECK_IN_RANGE(confstr(_CS_GNU_LIBPTHREAD_VERSION, name, bufsize), 1, bufsize);
	printf("confstr(_CS_GNU_LIBPTHREAD_VERSION) returns [%s]\n", name);
	return EXIT_SUCCESS;
}
