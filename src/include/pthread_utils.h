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

#ifndef __pthread_utils_h
#define __pthread_utils_h

/*
 * This is a collection of utils to help you deal with pthread issues.
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <pthread.h>	// for pthread_getattr_np(3), pthread_attr_getstack(3), pthread_attr_t
#include <unistd.h>	// for getpagesize(2)
#include <string.h>	// for memset(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()

/*
 * get the current threads stack size
 */
static inline void pthread_getstack(void** stackaddr, size_t* stacksize) {
	pthread_attr_t at;
	CHECK_ZERO_ERRNO(pthread_getattr_np(pthread_self(), &at));
	CHECK_ZERO_ERRNO(pthread_attr_getstack(&at, stackaddr, stacksize));
}

/*
 * prefault the entire stack for the current thread
 * TODO:
 * - the 0.9 in the next function is a cop-out. Find where we are
 * at the stack, find where the stack begins, calculate how much
 * there is to prefault and prefault it
 */
static inline void pthread_stack_prefault() {
	void* stackaddr;
	size_t stacksize;
	pthread_getstack(&stackaddr, &stacksize);
	// old code
	/* prefault less than the stacksize because when we were called some
	 * of the stack was already used... */
	/*
	 * size_t prefault_size=(int)(stacksize*0.9);
	 * unsigned char dummy[prefault_size];
	 * memset(&dummy, 0, prefault_size);
	 */
	// new code
	int a;
	char* pa=(char*)&a;
	size_t diff=pa-(char*)stackaddr;
	size_t prefault_size=diff-getpagesize();
	unsigned char dummy[prefault_size];
	memset(&dummy, 0, prefault_size);
}

/*
 * This is a function which is not supplied by the pthread API.
 * It gives you the recursion level of a pthread mutex.
 */
static inline int pthread_mutex_get_counter(const pthread_mutex_t * mutex) {
	// the ugly way...
	// return ((int*)&mylock)[1];
	// the nice way (although not official API)...
	return mutex->__data.__count;
}

#endif	/* !__pthread_utils_h */
