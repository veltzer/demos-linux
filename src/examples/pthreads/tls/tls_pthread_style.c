/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <pthread.h>	// for pthread_key_t, pthread_key_create(3), pthread_setspecific(3), pthread_getspecific(3), pthread_create(3), pthread_join(3)
#include <stdlib.h>	// for EXIT_SUCCESS, malloc(3), free(3)
#include <trace_utils.h>// for TRACE()
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_ZERO_ERRNO()

/*
 * This is a demo of thread local storage using the pthread API.
 *
 * TODO:
 * - destroy the ptherad_key_t...
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

pthread_key_t key_myid;

void* worker(void* arg) {
	CHECK_ZERO_ERRNO(pthread_setspecific(key_myid, arg));
	// now lets pull our id
	int myid=*(int*)pthread_getspecific(key_myid);
	TRACE("my id is %d\n", myid);
	return NULL;
}

void id_dealloc(void* ptr) {
	TRACE("deleting %p", ptr);
	int* p=(int*)ptr;
	free(p);
}

int main(int argc, char** argv, char** envp) {
	unsigned int i;
	const unsigned int num=4;
	pthread_t threads[num];
	TRACE("start");
	CHECK_ZERO_ERRNO(pthread_key_create(&key_myid, id_dealloc));
	for(i=0; i<num; i++) {
		int* p=(int*)malloc(sizeof(int));
		TRACE("allocated %p", p);
		CHECK_ZERO_ERRNO(pthread_create(threads + i, NULL, worker, p));
	}
	TRACE("created threads, now joining...");
	for(i=0; i<num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], NULL));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
