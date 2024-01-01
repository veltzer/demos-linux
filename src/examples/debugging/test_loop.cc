/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2024 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is just a loop that you can use to check debugger features.
 *
 * Example:
 * - run this example under gdb, then press CTRL+C, gdb will receive
 * SIG_INT, which it will use to pause the process. the you can
 * continue the process from within gdb using "c" or "continue".
 *
 * EXTRA_COMPILE_FLAGS=-g3
 */

int main(int argc, char** argv, char** envp) {
	volatile long sum=0;
	for(long i=0; i<1000000; i++) {
		for(long j=0; j<1000000000; j++) {
			sum+=i+j;
		}
	}
	printf("sum is %ld\n", sum);
	return EXIT_SUCCESS;
}
