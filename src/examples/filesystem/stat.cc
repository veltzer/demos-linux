/*
 * This file is part of the linuxapi package.
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
#include <stdio.h>	// for stderr, fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE

/*
 * This example is similar to the command line stat(1) one and
 * is similar in essence to the example in the stat(2) manual page.
 */

int main(int argc, char** argv, char** envp) {
	if(argc<2) {
		fprintf(stderr, "%s: usage: %s [file] [file] ...\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
