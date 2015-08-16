/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2015 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <pthread.h>	// for pthread_mutex_init(3), pthread_mutex_t(o)
#include <stdlib.h>	// for EXIT_SUCCESS, NULL
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()

/*
 * This example explores what default protocol is used by ptread_mutex.
 * It seems that this is not explicitly stated by:
 * pthread_mutexattr_getprotocol(3)
 * pthread_mutexattr_setprotocol(3)
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

int main(int argc, char** argv, char** envp) {
	pthread_mutex_t mylock;
	CHECK_ZERO_ERRNO(pthread_mutex_init(&mylock, NULL));
	return EXIT_SUCCESS;
}
