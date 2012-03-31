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

#include<errno.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h> // for EXIT_SUCCESS
#include<string.h>

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

	if ((fdindex = open("lpindex", O_RDWR|O_CREAT, 0666)) == -1)
	{
		perror("open lpindex file failed");
		exit(errno);
	}
	lplock.l_type = F_WRLCK;
	lplock.l_whence = SEEK_SET;
	lplock.l_start = 0;
	lplock.l_len = 0;
	if (fcntl(fdindex, F_SETLKW, & lplock) == -1)
	{
		perror("fcntl F_SETLK WriteLock failed");
		exit(errno);
	}
	if (fstat(fdindex, & buff) == -1)
	{
		perror("fstat fdindex failed");
		exit(errno);
	}
	if (buff.st_size == 0)
	{
		currid = 0;
		if (write(fdindex, & currid, sizeof(currid)) == -1)
		{
			perror("write initialize file failed");
			exit(errno);
		}
		buffer.ID = 0;
		buffer.path[0] = '\0';
		if (write(fdindex, & buffer, sizeof(buffer)) == -1)
		{
			perror("write initialize 2 failed");
			exit(errno);
		}
	}
	if (read(fdindex, & currid, sizeof(int)) == -1)
	{
		perror("read nextid failed");
		exit(errno);
	}
	if ((bufsize = read(fdindex, &buffer, sizeof(buffer))) == -1)
	{
		perror("read verify failed");
		exit(0);
	}
	while(buffer.ID == 0 && bufsize > 0)
	{
		if ((bufsize = read(fdindex, &buffer, sizeof(buffer))) == -1)
		{
			perror("read verify failed");
			exit(0);
		}
	}
	if (buffer.ID > 0)
	{
		printf("Now printing job: %d file: %s\n", buffer.ID, buffer.path);
	}
	if (lseek(fdindex, -1 * sizeof(buffer), SEEK_CUR) == -1)
	{
		perror("unable to revert to clear job ID");
		exit(errno);
	}
	buffer.ID = 0;
	if (write(fdindex, & buffer, sizeof(buffer)) == -1)
	{
		perror("Unable to clear job ID");
		exit(errno);
	}
	lplock.l_type = F_UNLCK;
	if (fcntl(fdindex, F_SETLK, & lplock) == -1)
	{
		perror("fcntl F_SETLK F_UNLK failed");
		exit(errno);
	}
	return EXIT_SUCCESS;
}
