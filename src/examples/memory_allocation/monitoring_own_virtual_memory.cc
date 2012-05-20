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
#include<stdlib.h> // for malloc(3), EXIT_SUCCESS
#include<stdio.h> // for printf(3)
#include<sys/time.h> // for getrusage(2)
#include<sys/resource.h> // for getrusage(2)
#include<strings.h> // for bzero(3)
#include<us_helper.h> // for CHECK_NOT_NULL()

/*
* This is an example which shows how to monitor your own virtual memory using getrusage
*
* Note that if you do not zero the memory (see the bzero call below) then this program
* works much faster and does not generate and page faults and the usage stays the same.
*/

extern char etext, edata, end; /* The symbols must have some type, or "gcc -Wall" complains */

void show_vmem() {
	struct rusage usage;
	CHECK_NOT_M1(getrusage(RUSAGE_SELF,&usage));
	printf("usage.ru_maxrss=%lu\n",usage.ru_maxrss);
	printf("usage.ru_minflt=%lu\n",usage.ru_minflt);
	// the following values do not change and so I don't print them...
	/*
	void* p=sbrk(0);
	printf("p is %p\n",p);
	printf("\tprogram text (etext) %10p\n", &etext);
	printf("\tinitialized data (edata) %10p\n", &edata);
	printf("\tuninitialized data (end) %10p\n", &end);
	*/
}

int main(int argc,char** argv,char** envp) {
	// size of each chunk (10M in this example)
	const size_t chunk_size=10*1024*1024;
	const unsigned int num_chunks=50;
	for(unsigned int i=0;i<num_chunks;i++) {
		printf("trying to allocate block number %d\n",i);
		CHECK_NOT_NULL(malloc(chunk_size));
		show_vmem();
	}
	return EXIT_SUCCESS;
}
