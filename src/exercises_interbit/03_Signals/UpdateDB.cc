#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/dir.h>
#include<sys/param.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

struct index
{
	unsigned int ID;
	char path[MAXPATHLEN];
};

struct index buffer;
int bufsize;
int fdindex;
int cri1done;
int cri2done;
int currid;

void critical1()
{
	char lpindexFname[MAXNAMLEN];
	struct stat buff;
	sprintf(lpindexFname, "/tmp/lpindex.%d", getpid());
	if ((fdindex = open(lpindexFname, O_RDWR|O_CREAT, 0666)) == -1)
	{
		perror("open lpindex file failed");
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
}

void critical2()
{
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
}

void sigint(int gotsig)
{
	DIR* sdir=NULL;
	struct dirent* dircontent;
	char pathname[MAXPATHLEN];
	struct stat statbuf;
	char strPID[10];
	if (!(sdir = opendir("/tmp")))
	{
		perror("opendir /tmp failed");
		exit(errno);
	}
	if (cri1done && ! cri2done)
	{
		critical2();
	}
	while ((dircontent = readdir(sdir)))
	{
		if ((strcmp(dircontent->d_name, "." ) == 0) 
			|| strcmp(dircontent->d_name, "..") == 0)
			continue;
		sprintf(strPID, "%d", getpid());
		if (strstr(dircontent->d_name, strPID))
		{
			sprintf(pathname, "/tmp/%s", dircontent->d_name);
			if (lstat(pathname, & statbuf) == -1)
			{
				perror(pathname);
				exit(errno);
			}
			if (S_ISREG(statbuf.st_mode)) {
				if (statbuf.st_uid == getuid())
				{
					if (unlink(pathname) == -1)
					{
						perror("unable to remove file");
						exit(errno);
					}
				}
			}
		}
	}
}

int main(int argc,char** argv,char** envp)
{
	sigset_t currentset, settoblock;
	struct sigaction act;
	sigemptyset(&settoblock);
	sigemptyset(&currentset);
	sigaddset(&settoblock, SIGINT);
	sigaddset(&settoblock, SIGQUIT);
	sigaddset(&settoblock, SIGABRT);
	sigaddset(&settoblock, SIGTERM);
	act.sa_handler = sigint;
	act.sa_mask = settoblock;
	if (sigaction(SIGINT,&act, NULL) == -1 ||
		sigaction(SIGQUIT, &act, NULL) == -1 ||
		sigaction(SIGABRT, &act, NULL) == -1 ||
		sigaction(SIGTERM, &act, NULL) == -1)
	{
		perror("sigaction failed");
		exit(errno);
	}

	cri1done = cri2done = 0;
	printf("blocking signals\n");
	if(sigprocmask(SIG_BLOCK, &settoblock, &currentset)!=0) {
		perror("cannot call sigprocmask");
		exit(1);
	}
	printf("doing critical 1\n");
	critical1();
	cri1done = 1;
	sleep(15);
	printf("doing critical 2\n");
	critical2();
	cri2done = 1;
	if(sigprocmask(SIG_SETMASK, &currentset, NULL)!=0) {
		perror("cannot call sigprocmask");
		exit(1);
	}
	fprintf(stderr,"enabling signals\n");
	return 0;
}
