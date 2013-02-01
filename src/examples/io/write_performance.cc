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

#include <firstinclude.h>
#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for malloc(3), atoi(3), free(3), EXIT_SUCCESS
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for close(2), write(2)
#include <us_helper.h> // for CHECK_NOT_M1(), run_high_priority(), getticks()
#include <stat.hh> // for Stat(O)

/*
* This example explores the performance of the write system call...
*
* The example is shows that writes to /dev/null have
* constant performance while writes to a file are different: when
* the OS buffers are empty they are fast (memcpy to kernel) and when
* they are full they block...
*
* example of running this could be:
* ./src/examples/io/write_performance.exe /tmp/foo 2000000 100 100 10000000 0
* You are supposed to see two peaks: one for fast writes which just copies to
* kernel and one for slow writes that block you...
*
* You can also use iotop to see the process consuming first place in the io
* category.
*
* EXTRA_LINK_FLAGS=-lpthread
*/

char* filename;
unsigned int bufsize;
unsigned int count;
unsigned int binnumber;
unsigned int binsize;
unsigned int binmean;

void* func(void*) {
	void* buf=malloc(bufsize);
	int fd;
	Stat s(binnumber,binsize,binmean);
	CHECK_NOT_M1(fd=open(filename,O_RDWR | O_CREAT,0666));
	for(unsigned int i=0;i<count;i++) {
		ticks_t t1=getticks();
		CHECK_NOT_M1(write(fd,buf,bufsize));
		ticks_t t2=getticks();
		s.accept(t2-t1);
	}
	CHECK_NOT_M1(close(fd));
	s.print();
	free(buf);
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	if(argc!=7) {
		fprintf(stderr,"usage: %s [filename] [bufsize] [count] [binnumber] [binsize] [binmean]\n",argv[0]);
		return -1;
	}
	filename=argv[1];
	bufsize=atoi(argv[2]);
	count=atoi(argv[3]);
	binnumber=atoi(argv[4]);
	binsize=atoi(argv[5]);
	binmean=atoi(argv[6]);
	run_high_priority(func,NULL,STANDARD_HIGH_PRIORITY);
	return EXIT_SUCCESS;
}
