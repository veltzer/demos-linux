/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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

#include <firstinclude.h>
#include <pthread.h> // for pthread_create(3). pthread_attr_init(3), pthread_attr_setinheritsched(3), pthread_attr_setschedpolicy(3), pthread_attr_setschedparam(3), pthread_attr_t, pthread_t
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for CHECK_ZERO()

/*
* This program calls pthread_create and tries to fail on purpse to see the error
* messages for this function.
*
* EXTRA_LINK_FLAGS=-lpthread
*/

void* myfunc(void*) {
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	no_params(argc,argv);

	pthread_attr_t myattr;
	pthread_attr_init(&myattr);
	pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
	struct sched_param myparam;
	// this is the bad argument to pthread_create(3)
	myparam.sched_priority=101;
	pthread_attr_setschedparam(&myattr, &myparam);

	pthread_t mythread;
	CHECK_ZERO(pthread_create(&mythread, &myattr, myfunc, NULL));
	return EXIT_SUCCESS;
}
