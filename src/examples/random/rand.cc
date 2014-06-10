/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdlib.h>	// for srandom(3), rand(3), EXIT_SUCCESS
#include <stdio.h>	// for sprintf(3)

/*
 * This example demos the pseudo random number generator
 * implemented in the glibc functions srandom etc.
 *
 * Note that since this is a pseudo random number generator
 * and it is always initialized using the same seed, then
 * it will always produce the EXACT series of "random"
 * numbers.
 */

int main(int argc, char** argv, char** envp) {
	srandom(1);
	for(unsigned int i=0; i<10; i++) {
		printf("i=%d, rand()=%d\n", i, rand());
	}
	return EXIT_SUCCESS;
}
