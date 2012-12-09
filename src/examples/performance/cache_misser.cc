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

#include <firstinclude.h>
#include <stdlib.h> // for malloc(3), EXIT_SUCCESS, rand(3)
#include <stdio.h> // for printf(3)

/*
* This is a sample which misses the cache on purpose...
*
* test this with:
* perf stat -e cache-misses ./src/examples/performance/cache_misser.exe
*/

int main(int argc,char** argv,char** envp) {
	const unsigned int size=1024*1024*100; // 100M buffer
	char* p=(char*)malloc(size);
	for(unsigned int i=0;i<size;i++) {
		p[i]=i%256;
	}
	long long sum=0;
	for(unsigned int i=0;i<100000000;i++) {
		int pos=rand()%size;
		sum+=p[pos];
	}
	printf("sum is %lld\n",sum);
	return EXIT_SUCCESS;
}
