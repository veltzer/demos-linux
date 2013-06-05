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

#ifndef __mymutex_h_
#define __mymutex_h

#include <firstinclude.h>
#include <pthread.h>	// for pthread_mutex_init(3), pthread_mutex_destroy(3)
#include <us_helper.h>	// for CHECK_ZERO_ERRNO()

class MyMutex {
private:
	pthread_mutex_t mylock;

public:
	MyMutex() {
		CHECK_ZERO_ERRNO(pthread_mutex_init(&mylock, NULL));
	}
	~MyMutex() {
		CHECK_ZERO_ERRNO(pthread_mutex_destroy(&mylock));
	}
	void lock() {
		CHECK_ZERO_ERRNO(pthread_mutex_lock(&mylock));
	}
	void unlock() {
		CHECK_ZERO_ERRNO(pthread_mutex_unlock(&mylock));
	}
};

#endif	// __mymutex_h
