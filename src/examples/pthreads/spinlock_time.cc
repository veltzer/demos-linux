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
#include <unistd.h>	// for sleep(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <pthread.h>	// for pthread_spin_init(3), pthread_spin_lock(3), pthread_spin_unlock(3), pthread_spin_destroy(3), pthread_create(3), pthread_join(3), pthread_spinlock_t, pthread_t
#include <us_helper.h>	// for TRACE(), CHECK_ZERO()

/*
 * This example examines the time it takes to lock and unlock a spin lock.
 * Since in this example the spin lock is never locked and since the thread
 * which examines this runs at a high priority then this should be a short
 * time indeed.
 *
 * TODO:
 * - enable to run this via high priority and show dead locks.
 * - show the results.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

static pthread_spinlock_t mylock;

static void *worker(void *not_used) {
	while(true) {
		TRACE("before lock");
		CHECK_ZERO(pthread_spin_lock(&mylock));
		CHECK_ZERO(sleep(1));
		CHECK_ZERO(pthread_spin_unlock(&mylock));
		TRACE("after lock");
		CHECK_ZERO(sleep(1));
	}
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	// first initialize the lock (no need for sharing between processes which
	// is the reason for the 0 in the second argument...)
	TRACE("initializing the lock...");
	CHECK_ZERO(pthread_spin_init(&mylock, 0));
	const int num=2;
	pthread_t threads[num];
	int ids[num];
	TRACE("starting threads...");
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("finished creating threads, joining them...");
	for (int i=0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	TRACE("joined all threads, destroying the lock...");
	CHECK_ZERO(pthread_spin_destroy(&mylock));
	return EXIT_SUCCESS;
}
