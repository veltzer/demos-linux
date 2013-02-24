/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <sys/types.h>	// for opendir(3), closedir(3), rewinddir(3)
#include <dirent.h>	// for opendir(3), readdir(3), closedir(3), rewinddir(3)
#include <stdlib.h>	// for qsort(3), EXIT_SUCCESS
#include <string.h>	// for strcasecmp(3), strncpy(3)
#include <sys/types.h>	// for lstat(2), getpwuid(2), for getgrgid(2)
#include <sys/stat.h>	// for lstat(2)
#include <unistd.h>	// for lstat(2)
#include <pwd.h>// for getpwuid(2)
#include <grp.h>// for getgrgid(2)
#include <math.h>	// for log10(3), lround(3)
#include <time.h>	// for localtime_r(3), strftime(3), time(2), difftime(3)
#include <us_helper.h>	// for CHECK_NOT_NULL(), CHECK_NOT_M1()

/*
 * TODO:
 * - date printing is not exactly as in ls as ls shows the date
 * for stuff which happened a year ago.
 * - print symlink.
 * - handle suid bits, gsetuid bits, tmpbits, ...
 * - adding colors?!?
 */

// ls considers anything older than 6 months to be printed with a year
// see 'info ls' for more info...
const double oldsecs=192*24*60*60;

static int strcmp_wrap(const void* pa, const void* pb) {
	const char** ppa=(const char**)pa;
	const char** ppb=(const char**)pb;
	return strcasecmp(*ppa, *ppb);
}

static void filetype(mode_t m, char* p) {
	if(S_ISREG(m)) {
		p[0]='-';
	}
	if(S_ISDIR(m)) {
		p[0]='d';
	}
	if(S_ISCHR(m)) {
		p[0]='c';
	}
	if(S_ISBLK(m)) {
		p[0]='b';
	}
	if(S_ISFIFO(m)) {
		p[0]='p';
	}
	if(S_ISLNK(m)) {
		p[0]='l';
	}
	if(S_ISSOCK(m)) {
		p[0]='s';
	}
	for(int i=1; i<10; i++) {
		p[i]='-';
	}
	if(S_IRUSR & m) {
		p[1]='r';
	}
	if(S_IWUSR & m) {
		p[2]='w';
	}
	if(S_IXUSR & m) {
		p[3]='x';
	}
	if(S_IRGRP & m) {
		p[4]='r';
	}
	if(S_IWGRP & m) {
		p[5]='w';
	}
	if(S_IXGRP & m) {
		p[6]='x';
	}
	if(S_IROTH & m) {
		p[7]='r';
	}
	if(S_IWOTH & m) {
		p[8]='w';
	}
	if(S_IXOTH & m) {
		p[9]='x';
	}
	p[10]='\0';
}

int main(int argc, char** argv, char** envp) {
	const bool hidedots=true;
	// lets take the current time
	time_t now;
	CHECK_NOT_M1(time(&now));
	DIR* d=(DIR*)CHECK_NOT_NULL(opendir("."));
	int num_files=0;
	int num_blocks=0;
	// no need to check the return code of readdir(3) (see the manual page for it...)
	struct dirent* dircontent;
	while((dircontent=readdir(d))) {
		if(hidedots && dircontent->d_name[0]=='.') {
			continue;
		}
		num_files++;
	}
	char** arr=(char**)malloc(sizeof(char*)*num_files);
	int i=0;
	// this function does not have an error statte
	rewinddir(d);
	int max_size=0;
	nlink_t max_link=0;
	while((dircontent=readdir(d))) {
		if(hidedots && dircontent->d_name[0]=='.') {
			continue;
		}
		arr[i]=(char*)malloc(256);
		strncpy(arr[i], dircontent->d_name, 256);
		struct stat buf;
		CHECK_NOT_M1(lstat(arr[i], &buf));
		num_blocks+=buf.st_blocks;
		if(buf.st_nlink>max_link) {
			max_link=buf.st_nlink;
		}
		if(buf.st_size>max_size) {
			max_size=buf.st_size;
		}
		i++;
	}
	CHECK_NOT_M1(closedir(d));
	int size_width=lrint(ceil(log10(max_size)));
	int size_link=lrint(ceil(log10(max_link+1)));
	qsort(arr, num_files, sizeof(char*), strcmp_wrap);
	printf("total %d\n", num_blocks/2);
	for(int i=0; i<num_files; i++) {
		// dont show . files...
		if(hidedots && arr[i][0]=='.') {
			continue;
		}
		struct stat buf;
		CHECK_NOT_M1(lstat(arr[i], &buf));
		char p[11];
		mode_t m=buf.st_mode;
		filetype(m, p);
		struct passwd *pwd=getpwuid(buf.st_uid);
		struct group* grp=getgrgid(buf.st_gid);
		struct tm mytm;
		CHECK_NOT_NULL(localtime_r(&buf.st_mtime, &mytm));
		const int date_buf_size=256;
		char mybuf[date_buf_size];
		const char* format_same_year="%b %e %R";
		const char* format_other_year="%b %e " " %Y";
		if(difftime(now, buf.st_mtime)<oldsecs) {
			CHECK_NOT_ZERO(strftime(mybuf, date_buf_size, format_same_year, &mytm));
		} else {
			CHECK_NOT_ZERO(strftime(mybuf, date_buf_size, format_other_year, &mytm));
		}
		printf("%s %*d %s %s %*ld %s %s\n",
			p,
			size_link,
			buf.st_nlink,
			pwd->pw_name,
			grp->gr_name,
			size_width,
			buf.st_size,
			mybuf,
			arr[i]
			);
	}
	return EXIT_SUCCESS;
}
