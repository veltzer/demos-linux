/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <unistd.h>	// for read(2), getdtablesize(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This is an example of what happens when you read from a bad fd.
 * The important thing to note is that you do not SEGFAULT. Technically
 * you called a syscall with the wrong parameter and what you get back
 * is an error.
 * Note yet that read(2) return type is ssize_t which is SIGNED as opposed to size_t
 * which is UNSIGNED. The idea here is the ability to return -1 and report errors
 * this way.
 */

int main(int argc, char** argv, char** envp) {
	const size_t SIZE=5;
	const int BAD_FD=getdtablesize()+1;
	char buf[SIZE];
	CHECK_NOT_M1(read(BAD_FD, buf, SIZE));
	return EXIT_SUCCESS;
}
