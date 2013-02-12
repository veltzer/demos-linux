/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for malloc(3)
#include <sys/time.h> // for gettimeofday(2)
#include <obstack.h> // for obstack_*(3)
#include <us_helper.h> // for CHECK_NOT_NULL()

/*
* This example shows how to use obstacks...
*
* Notes:
* - the obstack API is C so it can be used in both C and C++ apps.
* - notice the speed of obstack compared to the speed of malloc.
* because this is an uncontended case with just a single thread
* then malloc and obstack perform roughly the same (malloc
* on occasion a little slower...)
*
* TODO:
* - add multi threading and releases to really show the difference
* between obstacks and malloc.
*
*/

// these are needed so that obstacks would work fine...
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free
void* xmalloc(size_t size) {
	register void* value=CHECK_NOT_NULL(malloc(size));
	return value;
}

int main(int argc,char** argv,char** envp) {
	static struct obstack myobstack;
	obstack_init(&myobstack);
	struct timeval t1, t2;
	const unsigned int loop=100000;
	const unsigned int size_to_alloc=100;

	printf("testing obstack_alloc...\n");
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		obstack_alloc(&myobstack,size_to_alloc);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("testing malloc...\n");
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		void* p __attribute__((unused))=malloc(size_to_alloc);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return EXIT_SUCCESS;
}
