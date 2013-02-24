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
#include <stdio.h>	// for printf(3)
#include <sys/time.h>	// for gettimeofday(2)
#include <sys/types.h>	// for getpid(2), gettid(2)
#include <unistd.h>	// for getpid(2)
#include <stdlib.h>	// for free(3), malloc(3)
#include <pthread.h>	// for pthread_key_create(3), pthread_setspecific(3),
			// pthread_getspecific(3)
#include <us_helper.h>	// for micro_diff(), CHECK_NOT_M1()

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
 * How do I know that gcc actually calls getpid or gettid? I see it in the
 **disassemly.
 * (gettimeofday is obviously called)
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

pthread_key_t tid_key;
typedef struct _cached_tid {
	pid_t val;
} cached_tid;

void gettid_cache_delete(void* ptr) {
	free(ptr);
}

pid_t gettid_cached() {
	void* ptr=pthread_getspecific(tid_key);
	if(ptr==NULL) {
		cached_tid* ctid=(cached_tid*)malloc(sizeof(cached_tid));
		ctid->val=gettid();
		CHECK_NOT_M1(pthread_setspecific(tid_key, ctid));
		return ctid->val;
	} else {
		cached_tid* ctid=(cached_tid*)ptr;
		return ctid->val;
	}
}

void gettid_cache_init() {
	CHECK_NOT_M1(pthread_key_create(&tid_key, gettid_cache_delete));
}

int main(int argc, char** argv, char** envp) {
	struct timeval t1, t2;
	const unsigned int loop=1000000;

	printf("doing %d gettimeofday (for comparison)\n", loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0; i<loop; i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1, &t2)/(double)loop);

	printf("doing %d getpid\n", loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0; i<loop; i++) {
		getpid();
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1, &t2)/(double)loop);

	printf("doing %d gettid\n", loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0; i<loop; i++) {
		gettid();
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1, &t2)/(double)loop);

	gettid_cache_init();
	printf("doing %d gettid_cached\n", loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0; i<loop; i++) {
		gettid_cached();
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1, &t2)/(double)loop);
	return EXIT_SUCCESS;
}
