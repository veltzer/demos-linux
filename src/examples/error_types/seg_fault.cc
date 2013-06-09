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
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a demo of a segmentation fault created by calling a function badly.
 */

void func() {
	int i;
	const char* y __attribute__((unused))="123123dsfshfdkj$%^$%&^^%&@";
	for(i=0; i<100; i++) {
		fprintf(stderr, "i is %d\n", i);
	}
}

int main(int argc, char** argv, char** envp) {
	char* f=(char*)func;
	f++;
	f++;
	void (*g)()=(void (*)())f;
	g();
	return EXIT_SUCCESS;
}
