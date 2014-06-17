/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <err_utils.h>	// for CHECK_NOT_VOIDP
#include <unistd.h>	// for sleep(3), getpagesize(2)
#include <sys/mman.h>	// for mmap(2)
#include <stdio.h>	// for fprintf(3), stderr
#include <stdlib.h>	// for EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char** argv, char** envp) {
	int size=getpagesize();
	while(true) {
		CHECK_NOT_VOIDP(mmap(
			NULL,
			size,
			PROT_EXEC|PROT_READ|PROT_WRITE,
			MAP_ANONYMOUS|MAP_PRIVATE,
			-1,
			0
		), MAP_FAILED);
		sleep(1);
		size*=2;
	}
	return EXIT_SUCCESS;
}
