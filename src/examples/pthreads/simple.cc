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
#include <pthread.h>	// for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include <unistd.h>	// for sleep(3), getpid(2)
#include <sys/types.h>	// for getpid(2)
#include <sched.h>	// for sched_getcpu(2)
#include <us_helper.h>	// for CHECK_ZERO(), TRACE(), gettid(2)

/*
 * This is a standard pthread demo
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */
void *worker(void *p) {
	int num=*(int *)p;
	TRACE("starting thread %d", num);
	pthread_t t=pthread_self();
	TRACE("thread %d: pthread_self is %lu", num, t);
	TRACE("thread %d: gettid() is %d", num, gettid());
	TRACE("thread %d: getpid() is %d", num, getpid());
	TRACE("thread %d: sched_getcpu() is %d", num, sched_getcpu());
	CHECK_ZERO(sleep(60));
	TRACE("ending thread %d", num);
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	const int num=10;
	pthread_t threads[num];
	int ids[num];
	void* rets[num];

	TRACE("main starting");
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("main ended creating threads");
	for (int i=0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	TRACE("main ended");
	return EXIT_SUCCESS;
}
