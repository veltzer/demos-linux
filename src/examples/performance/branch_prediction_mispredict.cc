/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2016 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdlib.h>	// for EXIT_SUCCESS, rand(3)
#include <stdio.h>	// for printf(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)

/*
 * This example abuses the CPU as far as branch prediction goes...
 */

int main(int argc, char** argv, char** envp) {
	srand(getpid());
	long long sum=0;
	for(unsigned int i=0; i<100000000; i++) {
		if(rand()%2) {
			sum+=i*i;
		} else {
			sum-=i*i;
		}
	}
	printf("the sum is %lld\n", sum);
	return EXIT_SUCCESS;
}
