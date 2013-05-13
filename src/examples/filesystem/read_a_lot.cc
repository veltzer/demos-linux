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
#include <stdio.h> // for fprintf(3), fopen(3)
#include <stdlib.h> // for malloc(3), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h> // for pause(2), getpid(2), sleep(3), read(2)
#include <sys/types.h> // for getpid(2), open(2)
#include <sys/mman.h> // for mprotect(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <us_helper.h> // for CHECK_NOT_M1(), CHECK_INT()

/*
 * This example shows that you can continue to use a file after it has
 * been removed from the hard disk. So: as long as a process is using
 * a file its content will not be deleted from the hard drive.
 *
 * You must create the /tmp/myfile.data file, make sure it has
 * 3 bytes and a number int. Remove it as this process runs to
 * see it in action.
 */

int main(int argc,char** argv,char** envp) {
	int fd=CHECK_NOT_M1(open("/tmp/myfile.data",O_RDONLY));
	const unsigned int bufsize=1024;
	const unsigned int readsize=3;
	char buffer[bufsize];
	unsigned int counter=0;
	while(true) {
		CHECK_INT(read(fd,buffer,bufsize),readsize);
		int a;
		CHECK_INT(sscanf(buffer,"%d",&a),1);
		fprintf(stderr,"number is %d, counter is %d\n",a,counter);
		counter++;
		sleep(1);
		CHECK_NOT_M1(lseek(fd,0,SEEK_SET));
	}
	CHECK_NOT_M1(close(fd));
	return EXIT_SUCCESS;
}
