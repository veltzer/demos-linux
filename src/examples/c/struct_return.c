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
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an attempt to return a struct from a function
 */

typedef struct _foo{
	int a;
	int b;
} foo;

foo giveMeFoo(void) {
	// following line causes a compilation error...
	// return {3,4};
	foo f;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
	// cppcheck-suppress uninitvar
	return f;
#pragma GCC diagnostic pop
}

int main() {
	foo f=giveMeFoo();
	printf("f.a is %d\n", f.a);
	printf("f.b is %d\n", f.b);
	return EXIT_SUCCESS;
}
