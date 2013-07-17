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
#include <stdio.h>	// for fprintf(3), stderr
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_ASSERT()
#include <us_helper.h>	// for INFO(), readdir(), struct old_linux_dirent

/*
 * This example shows how to read the content of a directory using
 * the readdir(2) system call (old and deprecated way).
 *
 * A great part of this example was shamelessly stolen from the getdents(2)
 * manual page and adjusted for the readdir(2) system call.
 */

int main(int argc, char** argv, char** envp) {
	// command line parsing
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [dir]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s /etc\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// parameters
	const char* dirname=argv[1];
	// code
	int fd=CHECK_NOT_M1(open(dirname, O_RDONLY | O_DIRECTORY));
	struct old_linux_dirent d;
	printf("%8s %8s %12s %20s\n", "i-node#", "d_reclen", "d_off", "d_name");
	while(true) {
		int nread=CHECK_NOT_M1(readdir(fd, &d, sizeof(d)));
		if(nread==0) {
			break;
		}
		// readdir(2) only gives one result
		CHECK_ASSERT(nread==1);
		printf("%8ld %8d %12ld %20s\n", d.d_ino, d.d_reclen, d.d_off, d.d_name);
	}
	return EXIT_SUCCESS;
}
