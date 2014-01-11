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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <demangle.h>	// for cplus_demangle(3) 
#include <stdio.h>	// for printf(3)

/*
 * Demangle c++ symbols using libelf
 *
 * EXTRA_LINK_FLAGS=-liberty
 */

int main(int argc, char** argv, char** envp) {
	char* p=cplus_demangle("this is a mangled string", DMGL_GNU);
	printf("result is [%p]\n", p);
	return EXIT_SUCCESS;
}
