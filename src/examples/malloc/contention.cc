/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2016 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <sched.h>	// for cpu_set_t, CPU_ZERO(3), CPU_SET(3)
#include <unistd.h>	// for sysconf(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO(), CHECK_NOT_M1()
#include <stdlib.h>	// for EXIT_FAILURE, EXIT_SUCCESS, malloc(3), free(3)

/*
 * This example is designed to cause as much contention as possible
 * for the malloc(3) library and to investigate it's behaviour under
 * these conditions.
 *
 * TODO:
 * - Add the possibility to do more than one allocation at a time (do them in batches).
 * - Analyze the results of this example.
 * - Show that we block sometimes and sometimes we dont.
 * - Show that we can block on futex in user space and in kernel space on the mmap(2) semaphore.
 * - Show that we can play around with the blocking using MALLOC_ARENA_MAX.
 *
 * References:
 * - https://devcenter.heroku.com/articles/tuning-glibc-memory-behavior
 * - https://www.infobright.com/index.php/malloc_arena_max/#.VfhyG7PtD0o
 * - http://journal.siddhesh.in/posts/malloc-per-thread-arenas-in-glibc.html
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

static int num_threads;
static int num_iterations;
static int size_min;
static int size_max;

void *worker(void *p) {
	//int num=*(int *)p;
	int diff=size_max-size_min;
	for(int i=0;i<num_iterations;i++) {
		int size_to_alloc;
		if(diff==0) {
			size_to_alloc=size_min;
		} else {
			size_to_alloc=size_min+rand()%diff;
		}
		void* p=CHECK_NOT_NULL(malloc(size_to_alloc));
		// free(3) has not return value
		free(p);
	}
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	if(argc!=5) {
		fprintf(stderr, "%s: usage: %s [num_threads] [num_iterations] [size_min] [size_max]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 8 1048576 4096 4096\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	num_threads=atoi(argv[1]);
	num_iterations=atoi(argv[2]);
	size_min=atoi(argv[3]);
	size_max=atoi(argv[4]);

	const int cpu_num=CHECK_NOT_M1(sysconf(_SC_NPROCESSORS_ONLN));
	pthread_t threads[num_threads];
	pthread_attr_t attrs[num_threads];
	cpu_set_t cpu_sets[num_threads];
	int ids[num_threads];

	for(int i=0; i<num_threads; i++) {
		ids[i]=i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		CHECK_ZERO_ERRNO(pthread_attr_init(attrs + i));
		CHECK_ZERO_ERRNO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO_ERRNO(pthread_create(threads + i, attrs + i, worker, ids + i));
	}
	for(int i=0; i<num_threads; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], NULL));
	}
	return EXIT_SUCCESS;
}
