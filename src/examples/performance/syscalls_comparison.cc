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
#include <sys/time.h>	// for gettimeofday(2)
#include <sys/types.h>	// for getpid(2), gettid(2)
#include <unistd.h>	// for getpid(2)
#include <stdlib.h>	// for free(3), malloc(3)
#include <pthread.h>	// for pthread_key_create(3), pthread_setspecific(3), pthread_getspecific(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO(),
#include <sched_utils.h>// for sched_run_priority(), SCHED_FIFO_HIGH_PRIORITY:const
#include <measure.h>	// for measure, measure_init(), measure_start(), measure_end(), measure_print()

/*
 * This demo shows that the performance of various syscalls.
 * we compare:
 * - gettimeofday(2). worst. always does it's thing and needs to read
 * the system time.
 * - gettid(2). intermediate. not cached.
 * - getpid(2). best since it is cached by glibc.
 * we also show the performance of gettid_cached which is a TLS cached version
 * of gettid and performs much better.
 *
 * How do I know that gcc actually calls getpid or gettid? I see it in the disassemly.
 * (gettimeofday is obviously called)
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

static pthread_key_t tid_key;

typedef struct _cached_tid {
	pid_t val;
} cached_tid;

static void gettid_cache_delete(void* ptr) {
	free(ptr);
}

static pid_t gettid_cached() {
	void* ptr=pthread_getspecific(tid_key);
	if(ptr==NULL) {
		cached_tid* ctid=(cached_tid*)malloc(sizeof(cached_tid));
		ctid->val=gettid();
		CHECK_ZERO_ERRNO(pthread_setspecific(tid_key, ctid));
		return ctid->val;
	} else {
		cached_tid* ctid=(cached_tid*)ptr;
		return ctid->val;
	}
}

static void gettid_cache_init() {
	CHECK_ZERO_ERRNO(pthread_key_create(&tid_key, gettid_cache_delete));
}

static void* work(void* p) {
	const unsigned int loop=1000000;

	measure m;
	measure_init(&m, "gettimeofday", loop);
	measure_start(&m);
	for(unsigned int i=0; i<loop; i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
	}
	measure_end(&m);
	measure_print(&m);

	measure_init(&m, "getpid", loop);
	measure_start(&m);
	for(unsigned int i=0; i<loop; i++) {
		getpid();
	}
	measure_end(&m);
	measure_print(&m);

	measure_init(&m, "gettid", loop);
	measure_start(&m);
	for(unsigned int i=0; i<loop; i++) {
		gettid();
	}
	measure_end(&m);
	measure_print(&m);

	gettid_cache_init();
	measure_init(&m, "gettid_cached", loop);
	measure_start(&m);
	for(unsigned int i=0; i<loop; i++) {
		gettid_cached();
	}
	measure_end(&m);
	measure_print(&m);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	sched_run_priority(work, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	return EXIT_SUCCESS;
}
