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
#include <pthread.h>	// for pthread_t:struct, pthread_create(3), pthread_join(3)
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, rand(3), srand(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <atomic>	// for std::atomic

/*
 * This example explores the performance of rand(3) in a multi-threaded context.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 * for the atomics
 * EXTRA_COMPILE_FLAGS=-std=c++11
 */

static volatile bool stop=false;
std::atomic<int> counter;

static void *worker_rand(void *p) {
	while(!stop) {
		rand();
		counter++;
	}
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	// no errors from either getpid(2) or srand(3)
	srand(getpid());
	counter=0;
	const int num=10;
	pthread_t threads[num];
	int ids[num];
	void* rets[num];
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CHECK_ZERO_ERRNO(pthread_create(threads + i, NULL, worker_rand, ids + i));
	}
	CHECK_ZERO(sleep(5));
	stop=true;
	for (int i=0; i < num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], rets + i));
	}
	printf("number of random numbers generated is %d\n", (int)counter);
	return EXIT_SUCCESS;
}
