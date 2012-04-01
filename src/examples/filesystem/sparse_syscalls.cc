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

#include<stdio.h> // for perror(3)
#include<sys/types.h> // for open(2), lseek(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for write(2), lseek(2), close(2)
#include<stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE

/*
 * This example explores sparse files on a UNIX system.
 *
 * TODO:
 * - use my CHECK macros here.
 */
int main(int argc,char** argv,char** envp) {
	const char *buf = "hello";
	int fd = open("sparse_file", O_CREAT | O_RDWR, 0666);
	int res = fd;

	if (res == -1) {
		perror("error in open");
		exit(EXIT_FAILURE);
	}
	res = lseek(fd, 1000000, SEEK_CUR);
	if (res == -1) {
		perror("error in seek");
		exit(EXIT_FAILURE);
	}
	res = write(fd, buf, 5);
	if (res == -1) {
		perror("error in write");
		exit(EXIT_FAILURE);
	}
	res = close(fd);
	if (res == -1) {
		perror("error in close");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
