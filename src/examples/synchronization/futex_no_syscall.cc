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
#include <pthread.h>	// for pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_init(3), pthread_mutex_destory(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()
#include <us_helper.h>	// for TRACE()

/*
 * This example creates a pthread_mutex which is a futex, grabs it and releases
 * it. Run it with strace to see that it does not call any system call.
 *
 * Notes:
 * - there is no syscall involved with this futex AT ALL! This is because this futex is private
 *	and therefore not robust. Futexes can be robust in which case you need to tell that at creation
 *	which will notify the kernel about this futex.
 *	by default futexes are private.
 *	This means that even creation of futexes is cheap.
 *
 * Problem:
 * - even if I create a process shared mutex the pthread library calls no syscall at mutex_init time!!!
 *	How can this be if the futex is robust?!? investigate...
 *
 * EXTRA_LINK_FLAGS=-lpthread
 *
 * TODO:
 * - make this example strace itself...
 */

int main(int argc, char** argv, char** envp) {
	// TRACE("started");
	// this is the default type of locked (the "FAST" kind...) using the
	// special initialisation syntax...
	// pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

	// this is creation of pthread using the init function...
	// pthread_mutex_t mutex;
	// CHECK_ZERO_ERRNO(pthread_mutex_init(&mutex,NULL));

	// this is a shared mutex version
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
	CHECK_ZERO_ERRNO(pthread_mutexattr_init(&attr));
	CHECK_ZERO_ERRNO(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
	// CHECK_ZERO_ERRNO(pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_PRIVATE));
	CHECK_ZERO_ERRNO(pthread_mutex_init(&mutex, &attr));

	printf("before critical section\n");
	CHECK_ZERO_ERRNO(pthread_mutex_lock(&mutex));
	/*
	 * printf("here is a bug...\n");
	 * CHECK_ZERO_ERRNO(pthread_mutex_lock(&mutex));
	 */
	// TRACE("mutex address is %p", &mutex);
	printf("in critical section\n");
	CHECK_ZERO_ERRNO(pthread_mutex_unlock(&mutex));
	printf("after critical section\n");
	CHECK_ZERO_ERRNO(pthread_mutex_destroy(&mutex));
	// //TRACE("ended");
	return EXIT_SUCCESS;
}
