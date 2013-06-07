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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for read(2), write(2), close(2), getpagesize()
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This example shows how to copy a copy file program using the select system
 * call.
 *
 * The idea is to have a buffer of a certain size. When there is room in the buffer
 * keep calling read. When there is data in the buffer keep calling write. Handle
 * the buffer in a circular way. No need for any synchronization in this implementation
 * since only one thread is doing all the work.
 *
 * TODO:
 * - implement this example.
 */

void copy_file(const char* filein, const char* fileout, const unsigned int bufsize) {
	char buf[bufsize];
	int fdin=CHECK_NOT_M1(open(filein, O_RDONLY|O_LARGEFILE, 0666));
	int fdout=CHECK_NOT_M1(open(fileout, O_WRONLY|O_CREAT|O_TRUNC|O_LARGEFILE, 0666));
	// this is the main copy loop
	// we go out of the loop because of error or eof
	// >0: would have kept us in the loop
	// =0: that is ok - it is end of file
	// -1: error
	ssize_t read_bytes;
	do {
		read_bytes=CHECK_NOT_M1(read(fdin, buf, bufsize));
		char* p=buf;
		ssize_t len=read_bytes;
		while(len>0) {
			size_t written_bytes=CHECK_NOT_M1(write(fdout, p, len));
			len-=written_bytes;
			p+=written_bytes;
		}
	} while(read_bytes>0);
	CHECK_NOT_M1(close(fdin));
	CHECK_NOT_M1(close(fdout));
}

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [infile] [outfile] [num_pages]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const char* filein=argv[1];
	const char* fileout=argv[2];
	const unsigned int num_pages=atoi(argv[3]);
	copy_file(filein, fileout, num_pages*getpagesize());
	return EXIT_SUCCESS;
}
