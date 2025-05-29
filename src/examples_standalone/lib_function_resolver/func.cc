/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2025 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdlib.h>	// for srand(3), rand(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)

int func_imp1(int a, int b) {
	printf("in func_imp1\n");
	return a+b;
}

int func_imp2(int a, int b) {
	printf("in func_imp2\n");
	return a-b;
}

// the resolver function must be a C function
extern "C" {
static int (*resolve_func ())(int, int){
	printf("selecting implementation now...\n");
	srand(getpid());
	if(rand()%2) {
		return func_imp1;
	} else {
		return func_imp2;
	}
}
}

int func(int, int) __attribute__ ((ifunc ("resolve_func")));
