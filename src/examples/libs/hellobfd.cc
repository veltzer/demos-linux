/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<bfd.h>
#include<iostream>
#include<stdlib.h>

/*
 * This program demos the libbfd library which enabled you
 * to scan and manipulate object files in various formats
 *
 * EXTRA_LIBS=-lbfd
 */
int main(int argc, char** argv, char** envp) {
	bfd_init();
	bfd *b = bfd_openr("/bin/ls", NULL);
	if (!b) {
		std::cerr << "problem with open\n";
		return(1);
	}
	bfd_format format = bfd_object;
	bool ok_format = bfd_check_format(b, format);
	if (!ok_format) {
		std::cerr << "problem with bfd_check_format\n";
		return(1);
	}
	const char *name = bfd_format_string(format);
	std::cout << "format is " << name << "\n";
	bool res = bfd_close(b);
	if (!res) {
		std::cerr << "problem with close\n";
		return(1);
	}
	return EXIT_SUCCESS;
}
