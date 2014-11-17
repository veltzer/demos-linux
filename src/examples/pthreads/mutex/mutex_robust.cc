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
#include <unistd.h>	// for fork(2), getpagesize(2), sleep(3)
#include <pthread.h>	// for pthread_mutexattr_init(3), pthread_mutexattr_destroy(3), pthread_mutex_init(3), pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_destroy(3), PTHREAD_PROCESS_SHARED
#include <err_utils.h>	// for CHECK_ZERO_ERRNO(), CHECK_NOT_M1(), CHECK_NOT_VOIDP()
#include <sys/mman.h>	// for mmap(2), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANONYMOUS, MAP_FAILED
#include <trace_utils.h>// for TRACE()

/*
 * This example explores the robustness of futexes.
 * The algorithm:
 * Create two processes with shared memory that hold a futex in shared memory.
 * One of the processes locks the lock and crashes.
 * The other, currently, does not break from the dead lock.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

int main(int argc, char** argv, char** envp) {
	// setup shared memory where the futex will be
	void* shared=CHECK_NOT_VOIDP(mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0), MAP_FAILED);

	pthread_mutexattr_t attr;
	CHECK_ZERO_ERRNO(pthread_mutexattr_init(&attr));
	CHECK_ZERO_ERRNO(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));

	pthread_mutex_t* mylock=(pthread_mutex_t*)shared;
	// create the mutex with the SHARED attribute
	CHECK_ZERO_ERRNO(pthread_mutex_init(mylock, &attr));
	//CHECK_ZERO_ERRNO(pthread_mutexattr_destroy(&attr));
	if(CHECK_NOT_M1(fork())) {
		TRACE("parent");
		sleep(1);
		TRACE("parent after sleep");
		CHECK_ZERO_ERRNO(pthread_mutex_lock(mylock));
		TRACE("parent after lock");
		TRACE("parent dies");
	} else {
		TRACE("child");
		CHECK_ZERO_ERRNO(pthread_mutex_lock(mylock));
		TRACE("child after lock");
		sleep(2);
		TRACE("child after sleep");
		*(volatile int*)0=0;
		TRACE("child after crash?!?");
		CHECK_ZERO_ERRNO(pthread_mutex_unlock(mylock));
		TRACE("child dies");
	}
	return EXIT_SUCCESS;
}
