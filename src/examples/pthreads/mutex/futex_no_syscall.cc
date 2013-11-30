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
#include <multiproc_utils.h>	// for my_system(3)

/*
 * This example creates a pthread_mutex which is a futex, grabs it and releases
 * it. Run this example. It strace(1)s itself and you will see no system call
 * when acquiring and releasing the lock.
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
 */

static void print_mutexattr_data(pthread_mutexattr_t* attr) {
	int pshared, robust, type;
	CHECK_ZERO_ERRNO(pthread_mutexattr_getrobust(attr, &robust));
	CHECK_ZERO_ERRNO(pthread_mutexattr_getpshared(attr, &pshared));
	CHECK_ZERO_ERRNO(pthread_mutexattr_gettype(attr, &type));
	if(robust==PTHREAD_MUTEX_STALLED) {
		printf("robust is PTHREAD_MUTEX_STALLED\n");
	}
	if(robust==PTHREAD_MUTEX_ROBUST) {
		printf("robust is PTHREAD_MUTEX_ROBUST\n");
	}
	if(pshared==PTHREAD_PROCESS_SHARED) {
		printf("pshared is PTHREAD_PROCESS_SHARED\n");
	}
	if(pshared==PTHREAD_PROCESS_PRIVATE) {
		printf("pshared is PTHREAD_PROCESS_PRIVATE\n");
	}
	if(type==PTHREAD_MUTEX_FAST_NP) {
		printf("type is PTHREAD_MUTEX_FAST_NP\n");
	}
	if(type==PTHREAD_MUTEX_RECURSIVE_NP) {
		printf("type is PTHREAD_MUTEX_RECURSIVE_NP\n");
	}
	if(type==PTHREAD_MUTEX_ERRORCHECK_NP) {
		printf("type is PTHREAD_MUTEX_ERRORCHECK_NP\n");
	}
}

static int arr_robust[]={
	PTHREAD_MUTEX_STALLED,
	PTHREAD_MUTEX_ROBUST
};

static int arr_pshared[]={
	PTHREAD_PROCESS_SHARED,
	PTHREAD_PROCESS_PRIVATE
};

static int arr_type[]={
	PTHREAD_MUTEX_FAST_NP,
	PTHREAD_MUTEX_RECURSIVE_NP,
	PTHREAD_MUTEX_ERRORCHECK_NP
};

int main(int argc, char** argv, char** envp) {
	if(argc>1) {
		const int size=ARRAY_SIZEOF(arr_type)*ARRAY_SIZEOF(arr_pshared)*ARRAY_SIZEOF(arr_robust);
		pthread_mutex_t mtxs[size];
		pthread_mutexattr_t attrs[size];
		int i=0;
		for(unsigned int irobust=0; irobust<ARRAY_SIZEOF(arr_robust); irobust++) {
			for(unsigned int ipshared=0; ipshared<ARRAY_SIZEOF(arr_pshared); ipshared++) {
				for(unsigned int itype=0; itype<ARRAY_SIZEOF(arr_type); itype++) {
					int robust=arr_robust[irobust];
					int pshared=arr_pshared[ipshared];
					int type=arr_type[itype];
					pthread_mutexattr_t* attr=attrs+i;
					CHECK_ZERO_ERRNO(pthread_mutexattr_init(attr));
					CHECK_ZERO_ERRNO(pthread_mutexattr_setrobust(attr, robust));
					CHECK_ZERO_ERRNO(pthread_mutexattr_setpshared(attr, pshared));
					CHECK_ZERO_ERRNO(pthread_mutexattr_settype(attr, type));
					CHECK_ZERO_ERRNO(pthread_mutex_init(mtxs+i, attr));
					i++;
				}
			}
		}
		printf("started\n");
		for(int i=0; i<size; i++) {
			pthread_mutex_t* mutex=mtxs+i;
			pthread_mutexattr_t* attr=attrs+i;
			print_mutexattr_data(attr);
			printf("before critical section\n");
			CHECK_ZERO_ERRNO(pthread_mutex_lock(mutex));
			printf("in critical section\n");
			CHECK_ZERO_ERRNO(pthread_mutex_unlock(mutex));
			printf("after critical section\n");
			CHECK_ZERO_ERRNO(pthread_mutex_destroy(mutex));
		}
		printf("ended\n");
	} else {
		my_system("strace %s argument", argv[0]);
	}
	return EXIT_SUCCESS;
}
