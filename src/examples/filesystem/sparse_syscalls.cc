/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2025 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <sys/types.h>	// for open(2), lseek(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for write(2), lseek(2), close(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <err_utils.h>	// for CHECK_NOT_M1()

/*
 * This example explores sparse files on a UNIX system.
 */

int main() {
	int fd=CHECK_NOT_M1(open("/tmp/sparse_file", O_CREAT | O_RDWR, 0666));
	CHECK_NOT_M1(lseek(fd, 1000000, SEEK_CUR));
	const char *buf="hello";
	CHECK_NOT_M1(write(fd, buf, strlen(buf)));
	CHECK_NOT_M1(close(fd));
	return EXIT_SUCCESS;
}
