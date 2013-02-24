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
#include <stdlib.h>	// for malloc(3), EXIT_SUCCESS, EXIT_FAILURE, rand(3), atoi(3)
#include <stdio.h>	// for printf(3), fprintf(3), stderr

/*
 * This is a sample which misses the cache on purpose...
 *
 * test this with:
 * perf stat -e cache-misses ./src/examples/performance/cache_misser.elf [val] [times]
 * try 104857600 as the value (100MB)
 * note that when you pass times=0 you will still get lots of cache misses.
 * Those are the cache misses to materialize the memory. Do it one time with times=0
 * and then whenever you increase times you will get the extra cache misses you are
 * generating.
 * make the value bigger to see more misses...
 *
 * TODO:
 * - allocate the memory using mmap(2) and MAP_POPULATE to get number of cache misses
 * before starting the run lower.
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=3) {
		fprintf(stderr, "%s: usage: %s [val] [times]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	unsigned int size=atoi(argv[1]);
	unsigned int times=atoi(argv[2]);
	char* p=(char*)malloc(size);
	for(unsigned int i=0; i<size; i++) {
		p[i]=i%256;
	}
	long long sum=0;
	for(unsigned int i=0; i<times; i++) {
		int pos=rand()%size;
		sum+=p[pos];
	}
	printf("sum is %lld\n", sum);
	return EXIT_SUCCESS;
}
