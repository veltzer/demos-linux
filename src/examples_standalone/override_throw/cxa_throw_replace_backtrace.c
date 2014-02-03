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

#define _GNU_SOURCE
#include <dlfcn.h>
#include <execinfo.h>
#include <stdio.h>

typedef void (*cxa_throw_type)(void *, void *, void (*) (void *));
cxa_throw_type orig_cxa_throw = 0;

void load_orig_throw_code()
{
	orig_cxa_throw = (cxa_throw_type) dlsym(RTLD_NEXT, "__cxa_throw");
}

void __cxa_throw (void *thrown_exception, void *pvtinfo, void (*dest)(void *)) {
	printf(" ################ DETECT A THROWN !!!!! #############\n");
	if (orig_cxa_throw == 0)
		load_orig_throw_code();
	{
		static int throw_count = 0;
		void *array[10];
		size_t size;

		size = backtrace(array, 10);
		fprintf(stderr, "#### EXCEPTION THROWN (#%d) ####\n", ++throw_count);
		backtrace_symbols_fd(array, size, 2); // 2 == stderr
	}
	orig_cxa_throw(thrown_exception, pvtinfo, dest);
}
