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
#include<errno.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE

const int MAXINDEXSIZE=128;

struct index {
	unsigned int ID;
	char path[MAXPATHLEN];
};

int main(int argc,char** argv,char** envp) {
	int fdindex;
	struct index buffer;
	int buffersize;
	int currid;
	struct flock lplock;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if ((fdindex=open("lpindex", O_RDWR|O_CREAT, 0666)) == -1) {
		perror("open lpindex file failed");
		exit(errno);
	}
	lplock.l_type=F_WRLCK;
	lplock.l_whence=SEEK_SET;
	lplock.l_start=0;
	lplock.l_len=0;
	if (fcntl(fdindex, F_SETLKW, & lplock) == -1) {
		perror("fcntl F_SETLK WriteLock failed");
		exit(errno);
	}
	if ((buffersize=read(fdindex, & currid, sizeof(int))) == -1) {
		perror("read nextid failed");
		exit(errno);
	}
	if (buffersize == 0 || currid > MAXINDEXSIZE) {
		currid=0;
	}
	if (lseek(fdindex, sizeof(int) + currid * sizeof(buffer), SEEK_SET) == -1) {
		perror("seek to nextid failed");
		exit(errno);
	}
	if (read(fdindex, &buffer, sizeof(buffer)) == -1) {
		perror("read verify failed");
		exit(0);
	}
	if(buffer.ID>0) {
		fprintf(stderr, "Index table full. Retry print later");
		exit(EXIT_FAILURE);
	}
	if(lseek(fdindex, sizeof(int) + currid * sizeof(buffer), SEEK_SET) == -1) {
		perror("seek to nextid failed");
		exit(errno);
	}
	buffer.ID=currid+1;
	strcpy(buffer.path,argv[1]);
	if (write(fdindex,&buffer,sizeof(buffer))==-1) {
		perror("write buffer failed");
		exit(errno);
	}
	printf("file %s spooled, JobID: %d\n", buffer.path, buffer.ID);
	if (lseek(fdindex, 0, SEEK_SET) == -1) {
		perror("seek to start failed");
		exit(errno);
	}
	currid++;
	if (write(fdindex, & currid, sizeof(int)) == -1) {
		perror("write nextid failed");
		exit(errno);
	}
	lplock.l_type=F_UNLCK;
	if (fcntl(fdindex, F_SETLK, & lplock) == -1) {
		perror("fcntl F_SETLK F_UNLK failed");
		exit(errno);
	}
	return EXIT_SUCCESS;
}
