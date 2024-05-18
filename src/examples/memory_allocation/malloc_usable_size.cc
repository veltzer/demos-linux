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
#include <stdlib.h>	// for malloc(3)
#include <malloc.h>	// for malloc_usable_size(3)
#include <err_utils.h>	// for CHECK_NOT_NULL()

/*
 * This example shows the use of the malloc_usable_size(3) function.
 *
 * The result is that on an Intel 64 bit architecture I get 24 bytes used by malloc
 * even for small allocations. On 32 bit I remember I got 16 bytes (but I may be wrong).
 */

int main() {
	void* p=CHECK_NOT_NULL(malloc(4));
	printf("malloc_usage_size is %zd\n", malloc_usable_size(p));
	return EXIT_SUCCESS;
}
