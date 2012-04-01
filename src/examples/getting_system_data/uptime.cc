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

#include<stdio.h> // for printf(3)
#include<string.h> // for strtok_r(3)
#include<sys/types.h> // for open(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for read(2), close(2)
#include<stdlib.h> // for stof(3)

#include<us_helper.h>

/*
 * This demo shows how to get the uptime of the machine...
 * This is not very suitable for very delicate and precise timings but still.
 */
void uptime(float *time1, float *time2) {
	// null the pointers
	*time1 = 0;
	*time2 = 0;
	// read the data from the /proc/uptime virtual file...
	const char* filename = "/proc/uptime";
	const unsigned int size = 256;
	char buf[size];
	int d, res;
	CHECK_NOT_M1(d = open(filename, O_RDONLY));
	CHECK_NOT_M1(res = read(d, buf, size));
	char *saveptr;
	char *ptr = strtok_r(buf, " ", &saveptr);
	*time1 = atof(ptr);
	ptr = strtok_r(NULL, " ", &saveptr);
	*time2 = atof(ptr);
	CHECK_NOT_M1(close(d));
}


int main(int argc,char** argv,char** envp) {
	float time1, time2;
	uptime(&time1, &time2);
	printf("time1 is %f\n", time1);
	printf("time2 is %f\n", time2);
	return EXIT_SUCCESS;
}
