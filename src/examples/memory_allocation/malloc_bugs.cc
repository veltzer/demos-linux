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
#include <unistd.h>	// for malloc(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)

int main(int argc, char** argv, char** envp) {
	char* p=(char*)malloc(20);
	for(int i=0;i<100000000;i++) {
		printf("going to write on address %d,%p...\n", i, p+i);
		p[i]=0;
	}
	return EXIT_SUCCESS;
}
