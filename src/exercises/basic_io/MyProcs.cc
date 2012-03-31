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
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/dir.h>
#include<sys/param.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS

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
	return EXIT_SUCCESS;
}
