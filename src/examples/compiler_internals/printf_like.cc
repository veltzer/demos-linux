/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdarg.h>	// for va_start(3), va_end(3), va_list(3)
#include <stdio.h>	// for vfprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * An exampple of using the compiler to inforce 'printf' like correct
 * argument type passing...
 */

void my_trace(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

void my_trace(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

int main(int argc, char** argv, char** envp) {
	// this will not compile
	// my_trace("2+2 is %d\n","4");
	// this one does compile
	my_trace("2+2 is %d\n", 4);
	return EXIT_SUCCESS;
}
