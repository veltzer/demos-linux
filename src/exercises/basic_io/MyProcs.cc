#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/dir.h>
#include<sys/param.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void scanthedir(char* dirname) {
	DIR* sdir=NULL;
	DIR* fddir=NULL;
	struct dirent* dircontent;
	struct dirent* fddircontent;
	struct stat statbuf;
	char tmpdir[MAXPATHLEN];
	char fddirname[MAXPATHLEN];
	char linkname[MAXPATHLEN];
	char linktarget[MAXPATHLEN];
	int linktargetsize;
	if(!(sdir = opendir(dirname))) {
		perror(dirname);
		exit(errno);
	}
	while((dircontent=readdir(sdir))) {
		if ((strcmp(dircontent->d_name, "." ) == 0)
			|| strcmp(dircontent->d_name, "..") == 0)
			continue;
		sprintf(tmpdir, "%s/%s", dirname, dircontent->d_name);
		if (lstat(tmpdir, & statbuf) == -1) {
			perror(tmpdir);
			exit(errno);
		}
		if (S_ISDIR(statbuf.st_mode)) {
			if (statbuf.st_uid == getuid()) {
				sprintf(fddirname, "%s/fd", tmpdir);
				printf("%s\n", dircontent->d_name);
				if (!(fddir = opendir(fddirname))) {
					perror(fddirname);
					exit(errno);
				}
				while((fddircontent = readdir(fddir))) {
					if ((strcmp(fddircontent->d_name, "." ) == 0)
						|| strcmp(fddircontent->d_name, "..") == 0)
						continue;
					sprintf(linkname, "%s/%s", fddirname, fddircontent->d_name);
					if ((linktargetsize = readlink(linkname, linktarget, sizeof(linktarget))) == -1) {
						perror("readlink failed");
						exit(errno);
					}
					linktarget[linktargetsize] = '\0';
					printf("\t%s --> %s\n", linkname, linktarget);
				}
			}
		}
	}
	closedir(sdir);
}

int main(int argc,char** argv,char** envp) {
	char dirname[MAXPATHLEN];
	strcpy(dirname,"/proc");
	scanthedir(dirname);
	return 0;
}
