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
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include <malloc.h>	// for malloc_stats(3)
#include <pthread.h>	// for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include <unistd.h>	// for sleep(3)
#include <stdio.h>	// for fprintf(3)
#include <us_helper.h>	// for TRACE()
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()

/*
 * This example shows the use of the malloc_stats(3) function.
 * In order to make it interesting it first does some multi threading
 * work with some memory allocations to make the pools/arenas appear.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

void *worker(void *p) {
	int num=*(int *)p;
	TRACE("starting thread %d", num);
	pthread_t t=pthread_self();
	int* pointer=(int*)&t;
	TRACE("pthread_self is %d", *pointer);
	// lets allocate and deallocate memory...
	const unsigned int size=1000;
	void* arr[size];
	for(unsigned int i=0; i<size; i++) {
		arr[i]=malloc(size);
	}
	for(unsigned int i=0; i<size; i++) {
		free(arr[i]);
	}
	TRACE("ending thread %d", num);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [num_threads]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const int num=atoi(argv[1]);
	pthread_t* threads=new pthread_t[num];
	int ids[num];
	void* rets[num];

	TRACE("main starting");
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CHECK_ZERO_ERRNO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("main ended creating threads");
	for (int i=0; i < num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], rets + i));
	}
	TRACE("main ended");
	malloc_stats();
	return EXIT_SUCCESS;
}
