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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_VOIDP()

/*
 * This example shows how to implement a simple copy file using two threads.
 * This is useful if the two files are on different hard drives and have different
 * read/write bandwidths.
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [infile] [outfile] [numpages]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	//const char* filein=argv[1];
	//const char* fileout=argv[2];
	//const unsigned int numpages=atoi(argv[3]);
	return EXIT_SUCCESS;
}
