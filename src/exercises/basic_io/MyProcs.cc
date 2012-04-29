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
#include<sys/stat.h> // for lstat(2)
#include<sys/dir.h>
#include<sys/param.h> // for MAXPATHLEN
#include<unistd.h>
#include<sys/types.h> // for opendir(3), DIR, struct dirent
#include<dirent.h> // for opendir(3), DIR, struct dirent
#include<stdio.h> // for snprintf(3)
#include<string.h> // for strcmp(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include<us_helper.h> // for CHECK_NOT_M1(), CHECK_NOT_NULL()

void scanthedir(const char* dirname) {
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
	CHECK_NOT_NULL(sdir = opendir(dirname));
	while((dircontent=readdir(sdir))) {
		if ((strcmp(dircontent->d_name, "." ) == 0)
			|| strcmp(dircontent->d_name, "..") == 0)
			continue;
		snprintf(tmpdir, MAXPATHLEN,"%s/%s", dirname, dircontent->d_name);
		CHECK_NOT_M1(lstat(tmpdir, & statbuf));
		if (S_ISDIR(statbuf.st_mode)) {
			if (statbuf.st_uid == getuid()) {
				snprintf(fddirname, MAXPATHLEN,"%s/fd", tmpdir);
				printf("%s\n", dircontent->d_name);
				CHECK_NOT_NULL(fddir = opendir(fddirname));
				while((fddircontent = readdir(fddir))) {
					if ((strcmp(fddircontent->d_name, "." ) == 0)
						|| strcmp(fddircontent->d_name, "..") == 0)
						continue;
					snprintf(linkname, MAXPATHLEN,"%s/%s", fddirname, fddircontent->d_name);
					CHECK_NOT_M1(linktargetsize=readlink(linkname, linktarget, sizeof(linktarget)));
					linktarget[linktargetsize] = '\0';
					printf("\t%s --> %s\n", linkname, linktarget);
				}
			}
		}
	}
	CHECK_NOT_M1(closedir(sdir));
}

int main(int argc,char** argv,char** envp) {
	scanthedir("/proc");
	return EXIT_SUCCESS;
}
