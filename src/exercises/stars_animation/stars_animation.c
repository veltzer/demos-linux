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
#include <stdio.h>	// for scanf(3), putchar(3), fflush(3)
#include <unistd.h>	// for usleep(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_INT(), CHECK_NOT_M1()

int main(int argc, char** argv, char** envp) {
	int num;
	printf("Give me a number of stars: ");
	fflush(stdout);
	CHECK_INT(scanf("%d", &num), 1);
	while(true) {
		int i;
		int u;
		for(i=0, u=num-1; i<num; i++, u--) {
			int j;
			for(j=0; j<u; j++) {
				putchar(' ');
			}
			for(j=0; j<i+1; j++) {
				putchar('*');
				putchar(' ');
			}
			for(j=0; j<u; j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			CHECK_NOT_M1(usleep(100000));
		}
		for(i=num-1, u=0; i>=0; i--, u++) {
			int j;
			for(j=0; j<u; j++) {
				putchar(' ');
			}
			for(j=0; j<i+1; j++) {
				putchar('*');
				putchar(' ');
			}
			for(j=0; j<u; j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			CHECK_NOT_M1(usleep(100000));
		}
	}
	return EXIT_SUCCESS;
}