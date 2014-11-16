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
#include <unistd.h>	// for fork(2), getpagesize(2)
#include <pthread.h>	// for pthread_mutex_init(3), pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_destroy(3), pthread_create(3), pthread_join(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO(), CHECK_ZERO(), CHECK_NOT_VOIDP()
#include <sys/mman.h>	// for mmap(2), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANONYMOUS, MAP_FAILED

/*
 * This example explores the robustness of futexes.
 * The idea:
 * Create two processes with shared memory that hold a futex in shared memory.
 * On of the processes locks the lock and crashes.
 * We want to see what the other will get.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

int main(int argc, char** argv, char** envp) {
	// setup shared memory where the futex will be
	void* shared=CHECK_NOT_VOIDP(mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0), MAP_FAILED);
	pthread_mutex_t* mylock=(pthread_mutex_t*)shared;
	// create the mutex with the SHARED attribute
	CHECK_ZERO_ERRNO(pthread_mutex_init(mylock, NULL));
	if(CHECK_NOT_M1(fork())) {
		// parent
	} else {
		// child
	}
	return EXIT_SUCCESS;
}
