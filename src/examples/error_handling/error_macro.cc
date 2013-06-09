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
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)

#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE

/*
 * This program errors on purpose.
 */

// #define CHK_NOT_M1(a) if(a==-1) { printf("error " # a); printf("\n"); exit(EXIT_FAILURE);}
#define CHK_NOT_M1(a) if(a==-1) { printf("error %s\n", "" # a); exit(EXIT_FAILURE); }

int main(int argc, char** argv, char** envp) {
	CHK_NOT_M1(open("thisfiledoesnotexist", O_RDONLY));
	return EXIT_SUCCESS;
}
