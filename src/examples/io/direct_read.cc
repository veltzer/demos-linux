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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, aligned_alloc(3), malloc(3)
#include <strings.h>	// for bzero(3)
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <err_utils.h>	// for CHECK_NOT_M1()

/*
 * This example explores the performance impact of the O_DIRECT
 * flag when working with a file.
 *
 * You can create a big file of size gigabyte in /tmp this way:
 * dd if=/dev/urandom of=/tmp/bigfile count=2097152
 *
 * TODO:
 * - show the difference in return values and in performance
 * between doing malloc and aligned malloc.
 * - show the difference in return values and in performance
 * between doing O_DIRECT and not.
 */

static bool use_malloc=false;
static bool use_direct=true;

int main(int argc, char** argv, char** envp) {
	const unsigned int size=1024*1024*1024;
	char* p;
	if(use_malloc) {
		p=(char*)malloc(size);
	} else {
		p=(char*)aligned_alloc(getpagesize(), size);
	}
	bzero(p, size);
	int fd;
	if(use_direct) {
		fd=CHECK_NOT_M1(open("/tmp/bigfile", O_RDONLY | O_DIRECT));
	} else {
		fd=CHECK_NOT_M1(open("/tmp/bigfile", O_RDONLY));
	}
	int ret=CHECK_NOT_M1(read(fd, p, size));
	printf("ret is %d\n", ret);
	CHECK_NOT_M1(close(fd));
	return EXIT_SUCCESS;
}
