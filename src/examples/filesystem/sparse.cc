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

#include<stdio.h> // for perror(3)
#include<unistd.h>
#include<string.h> // for memset(3)
#include<sys/types.h> // for stat(2)
#include<sys/stat.h> // for stat(2)
#include<unistd.h> // for stat(2)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, exit(3)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
 * This program demostrates the concept of sparse files...
 * Notice the following facts:
 * - this behaviour depends on the underlying file system and some brain
 * dead filesystems (like DOS) will not do any sparse thing.
 * - this bahaviour is DEFAULT in linux and unix on major supported filesystems.
 * - other pages are full of 0's.
 */

// this is the position we will seek to...
//const int pos=1024*1024;
const int pos = 1000000;
// this is the string to be printed into the file
const char *string = "hello";
// this is the name of the file to be used
const char *fname = "/tmp/my.sparse.file";

int main(int argc, char **argv, char **envp) {
	// this will be used to check return values
	int ret = 0;
	FILE *f = fopen(fname, "w");
	if (f == NULL) {
		perror("could not open file");
		exit(EXIT_FAILURE);
	}
	ret = fseek(f, pos, SEEK_CUR);
	if (ret == -1) {
		perror("could not seek file");
		exit(EXIT_FAILURE);
	}
	ret = fwrite(string, strlen(string), 1, f);
	if (ret != 1) {
		perror("could not write file");
		exit(EXIT_FAILURE);
	}
	ret = fclose(f);
	if (ret == -1) {
		perror("could not close file");
		exit(EXIT_FAILURE);
	}
	struct stat stat_buf;
	CHECK_NOT_M1(stat(fname, &stat_buf));
	fprintf(stderr,"buf.size is %lu\n",stat_buf.st_size);
	fprintf(stderr,"buf.blocks (512B each) is %lu\n",stat_buf.st_blocks);
	fprintf(stderr,"real total calculated size is %lu\n",stat_buf.st_blocks*512);
	my_system("ls -l %s",fname);
	my_system("du -h %s",fname);
	// now lets see that we can read from the file...
	f = fopen(fname, "r");
	if (f == NULL) {
		perror("could not open file");
		exit(EXIT_FAILURE);
	}
	const unsigned int buf_size=4096;
	char buf[buf_size];
	memset(buf,3,buf_size);
	memcheck(buf,3,buf_size);

	ret = fread(buf, buf_size, 1, f);
	if (ret != 1) {
		perror("could not read file");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr,"checking that the buffer is full of 0's\n");
	memcheck(buf,0,buf_size);

	// now lets get ridd of the file...
	CHECK_NOT_M1(unlink(fname));
	return EXIT_SUCCESS;
}
