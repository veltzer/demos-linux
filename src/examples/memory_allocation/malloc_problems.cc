/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for malloc(3), EXIT_SUCCESS
#include <unistd.h>	// for sleep(3)
#include <sys/mman.h>	// for mlock(2)

/*
 * This example shows that when the OS finally catches
 * you on an illegal access then the pointer is a page
 * bounded pointer
 */

int main(int argc, char** argv, char** envp) {
	int res=mlockall(MCL_CURRENT|MCL_FUTURE);
	if(res==-1) {
		fprintf(stderr, "mlockall failed\n");
		return EXIT_FAILURE;
	}
	char* p=(char*)malloc(20);
	/*
	int res=mlock(p,128*1024);
	if(res==-1) {
		fprintf(stderr, "mlock failed\n");
		return EXIT_FAILURE;
	}
	*/
	for(int i=0;i<10000000;i+=4096) {
		printf("i is %i, p+i is %p\n", i, p+i);
		p[i]=0;
		sleep(1);
	}
	return EXIT_SUCCESS;
}
