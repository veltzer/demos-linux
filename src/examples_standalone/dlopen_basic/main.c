/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2022 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdio.h> // for printf(3)
#include <dlfcn.h> // for dlopen(3), dlsym(3), dlclose(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <err_utils.h> // for CHECK_NOT_NULL(), CHECK_ZERO()

int main(int argc,char** argv,char** envp) {
	void* h=CHECK_NOT_NULL(dlopen("libadd.so", RTLD_NOW));
	void* sym=CHECK_NOT_NULL(dlsym(h,"func"));
	int (*f)(int, int);
	f=((int(*) (int, int))sym);
	int result=f(2, 2);
	printf("2+2 is %d\n", result);
	CHECK_ZERO(dlclose(h));
	return EXIT_SUCCESS;
}
