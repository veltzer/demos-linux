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

#include<firstinclude.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h> // for EXIT_SUCCESS
#include<string.h>
#include<us_helper.h> // for CHECK_NOT_M1()

const int MAXINDEXSIZE=128;

struct index {
	unsigned int ID;
	char path[MAXPATHLEN];
};

int main(int argc,char** argv,char** envp) {
	int fdindex;
	struct index buffer;
	int bufsize;
	int currid;
	struct flock lplock;
	struct stat buff;

	CHECK_NOT_M1((fdindex=open("lpindex", O_RDWR|O_CREAT, 0666)));
	lplock.l_type=F_WRLCK;
	lplock.l_whence=SEEK_SET;
	lplock.l_start=0;
	lplock.l_len=0;
	CHECK_NOT_M1(fcntl(fdindex, F_SETLKW, & lplock));
	CHECK_NOT_M1(fstat(fdindex, & buff));
	if (buff.st_size==0) {
		currid=0;
		CHECK_NOT_M1(write(fdindex, & currid, sizeof(currid)));
		buffer.ID=0;
		buffer.path[0]='\0';
		CHECK_NOT_M1(write(fdindex, & buffer, sizeof(buffer)));
	}
	CHECK_NOT_M1(read(fdindex, & currid, sizeof(int)));
	CHECK_NOT_M1((bufsize=read(fdindex, &buffer, sizeof(buffer))));
	while(buffer.ID==0 && bufsize > 0)
	{
		CHECK_NOT_M1(read(fdindex, &buffer, sizeof(buffer)));
	}
	if (buffer.ID > 0)
	{
		printf("Now printing job: %d file: %s\n", buffer.ID, buffer.path);
	}
	CHECK_NOT_M1(lseek(fdindex, -1 * sizeof(buffer), SEEK_CUR));
	buffer.ID=0;
	CHECK_NOT_M1(write(fdindex, & buffer, sizeof(buffer)));
	lplock.l_type=F_UNLCK;
	CHECK_NOT_M1(fcntl(fdindex, F_SETLK, & lplock));
	return EXIT_SUCCESS;
}
