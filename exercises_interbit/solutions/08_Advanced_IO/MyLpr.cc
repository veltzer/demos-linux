#include <errno.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAXINDEXSIZE=128;
struct index
{
	unsigned int ID;
	char path[MAXPATHLEN];
};

int main(int argc,char** argv,char** envp)
{
	int fdindex;
	struct index buffer;
	int buffersize;
	int currid;
	struct flock lplock;

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}
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
	if ((buffersize = read(fdindex, & currid, sizeof(int))) == -1)
	{
		perror("read nextid failed");
		exit(errno);
	}
	if (buffersize == 0 || currid > MAXINDEXSIZE)
	{
		currid = 0;
	}
	if (lseek(fdindex, sizeof(int) + currid * sizeof(buffer), SEEK_SET) == -1)
	{
		perror("seek to nextid failed");
		exit(errno);
	}
	if (read(fdindex, &buffer, sizeof(buffer)) == -1)
	{
		perror("read verify failed");
		exit(0);
	}
	if (buffer.ID > 0)
	{
		fprintf(stderr, "Index table full. Retry print later");
		exit(1);
	}
	if (lseek(fdindex, sizeof(int) + currid * sizeof(buffer), SEEK_SET) == -1)
	{
		perror("seek to nextid failed");
		exit(errno);
	}
	buffer.ID = currid + 1;
	strcpy(buffer.path, argv[1]);
	if (write(fdindex, & buffer, sizeof(buffer)) == -1)
	{
		perror("write buffer failed");
		exit(errno);
	}
	printf("file %s spooled, JobID: %d\n", buffer.path, buffer.ID);
	if (lseek(fdindex, 0, SEEK_SET) == -1)
	{
		perror("seek to start failed");
		exit(errno);
	}
	currid++;
	if (write(fdindex, & currid, sizeof(int)) == -1)
	{
		perror("write nextid failed");
		exit(errno);
	}
	lplock.l_type = F_UNLCK;
	if (fcntl(fdindex, F_SETLK, & lplock) == -1)
	{
		perror("fcntl F_SETLK F_UNLK failed");
		exit(errno);
	}
	return 0;
}
