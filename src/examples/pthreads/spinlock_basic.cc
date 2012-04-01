/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<pthread.h> // for pthread_spin_init/lock/unlock/destroy(3),
// for pthread_create(3), pthread_join(3)

#include<us_helper.h> // for TRACE()

/*
 * This is a demo for thread spin locks.
 * Run on any machine and you will see a problem in that they will block
 * each other. This blockage does not turn into a dead lock since the OS
 * will preempt them and eventually each will get it's turn.
 *
 * EXTRA_LIBS=-lpthread
 */

static pthread_spinlock_t mylock;

static void *worker(void *p) {
	for(int i=0;i<10;i++) {
		//TRACE("before lock");
		CHECK_ZERO(pthread_spin_lock(&mylock));
		TRACE("sleeping");
		sleep(1);
		CHECK_ZERO(pthread_spin_unlock(&mylock));
		//TRACE("after lock");
		sleep(1);
	}
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	// first initialize the lock (no need for sharing between processes which
	// is the reason for the 0 in the second argument...)
	TRACE("initializing the lock...");
	CHECK_ZERO(pthread_spin_init(&mylock, 0));
	const int num = 2;
	pthread_t threads[num];
	int ids[num];
	TRACE("starting threads...");
	for(int i=0;i<num;i++) {
		ids[i] = i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("finished creating threads, joining them...");
	for(int i=0;i<num;i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	TRACE("joined all threads, destroying the lock...");
	CHECK_ZERO(pthread_spin_destroy(&mylock));
	return EXIT_SUCCESS;
}
