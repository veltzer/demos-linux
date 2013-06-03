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
#include <unistd.h>	// for read(2), write(2), close(2)
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This demo shows how to correctly code a simple file copy using the
 * read(2) and write(2) system calls, handling short reads appropriately.
 */

void copy_file(const char* filein, const char* fileout) {
	const unsigned int bufsize=4096;
	char buf[bufsize];
	int fdin=CHECK_NOT_M1(open(filein, O_RDONLY, 0666));
	int fdout=CHECK_NOT_M1(open(fileout, O_WRONLY|O_CREAT|O_TRUNC, 0666));
	ssize_t read_bytes;
	// this is the main copy loop
	// we go out of the loop because of error or eof
	// >0: would have kept us in the loop
	// =0: that is ok - it is end of file
	// -1: error
	read_bytes=CHECK_NOT_M1(read(fdin, buf, bufsize));
	while(read_bytes>0) {
		char* pointer=buf;
		ssize_t written_bytes=0;
		ssize_t to_write=read_bytes;
		while(written_bytes<read_bytes) {
			ssize_t write_bytes=CHECK_NOT_M1(write(fdout, pointer, to_write));
			to_write-=write_bytes;
			pointer+=write_bytes;
			written_bytes+=write_bytes;
		}
		read_bytes=CHECK_NOT_M1(read(fdin, buf, bufsize));
	}
	CHECK_NOT_M1(close(fdin));
	CHECK_NOT_M1(close(fdout));
}

int main(int argc, char** argv, char** envp) {
	if(argc!=3) {
		fprintf(stderr, "%s: usage: %s [infile] [outfile]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const char* filein=argv[1];
	const char* fileout=argv[2];
	copy_file(filein, fileout);
	return EXIT_SUCCESS;
}
