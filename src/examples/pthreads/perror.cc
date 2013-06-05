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
#include <stdio.h>	// for fprintf(3), perror(3)
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <errno.h>	// for errno
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_ZERO_ERRNO(), TRACE()

/*
 * This is a demo which shows that perror, errno etc all work on a thread
 * by thread level...
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */
void *worker1(void *p) {
	TRACE("worker1 starting thread");
	TRACE("test 1: with no error");
	int err=errno;
	TRACE("errno is %d (%s)", err, strerror(err));
	TRACE("test 2: doing something illegal");
	open("/etc/nonexistant", O_RDWR);
	err=errno;
	TRACE("errno is %d (%s)", err, strerror(err));
	TRACE("test 3: setting error myself");
	errno=3;
	err=3;
	perror("msg from perror");
	TRACE("errno is %d (%s)", err, strerror(err));
	TRACE("worker1 ending thread");
	return(NULL);
}

void *worker2(void *p) {
	TRACE("worker2 starting thread");
	TRACE("errno is %d", errno);
	perror("printing perror");
	TRACE("worker2 ending thread");
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	pthread_t thread1, thread2;
	const int num=2;
	int ids[num];
	void* rets[num];

	CHECK_ZERO_ERRNO(pthread_create(&thread1, NULL, worker1, ids + 0));
	CHECK_ZERO_ERRNO(pthread_create(&thread2, NULL, worker2, ids + 1));
	CHECK_ZERO_ERRNO(pthread_join(thread1, rets + 0));
	CHECK_ZERO_ERRNO(pthread_join(thread2, rets + 1));
	return EXIT_SUCCESS;
}
