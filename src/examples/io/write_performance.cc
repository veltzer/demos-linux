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

#include<stdio.h> // for printf(3), fprintf(3)
#include<strings.h> // for bzero(3)
#include<stdlib.h> // for malloc(3), atoi(3)
#include<sys/types.h> // for open(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)

#include<us_helper.h> // our own helper
#include<stat.hh> // our own helper

/*
 * This example explores the performance of the write system call...
 *
 * The example is supposed to show that writes to /dev/null have
 * constant performance while writes to a file are different: when
 * the OS buffers are empty they are fast (memcpy to kernel) and when
 * they are full they block...
 *
 * EXTRA_LIBS=
 */
int main(int argc,char** argv,char** envp) {
	if(argc!=4) {
		fprintf(stderr,"usage: %s [filename] [bufsize] [count]\n",argv[0]);
		return -1;
	}
	const char* filename=argv[1];
	const unsigned int bufsize=atoi(argv[2]);
	const unsigned int count=atoi(argv[3]);
	void* buf=malloc(bufsize);
	int fd;
	Stat s(
		500, // bin number
		1000,
		1000
	);
	CHECK_NOT_M1(fd=open(filename,O_RDWR | O_CREAT,0666));
	for(unsigned int i=0;i<count;i++) {
		ticks_t t1=getticks();
		CHECK_NOT_M1(write(fd,buf,bufsize));
		ticks_t t2=getticks();
		s.accept(t2-t1);
	}
	CHECK_NOT_M1(close(fd));
	s.print();
	return EXIT_SUCCESS;
}
