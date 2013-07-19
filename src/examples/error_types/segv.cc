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
#include <unistd.h>	// for EXIT_SUCCESS
#include <trace_utils.h>// for TRACE()

/*
 * This is a simple example causing a segmentation fault to be generated.
 *
 * The address "0" is not mapped into any regular processes virtual address space
 * which is a good thing since NULL (which is heavily used in C programs) has the
 * value 0.
 *
 * TODO:
 * - show that you can catch SIGSEGV and even continue running.
 */

int main(int argc, char** argv, char** envp) {
	// lets show that the NULL constant is actually the number 0...
	TRACE("NULL is %zd", NULL);
	char *p=0;
	p[0]=0;
	return EXIT_SUCCESS;
}
