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

#include <firstinclude.h>
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <sys/types.h> // for gettid(2), getpid(2)
#include <unistd.h> // for getpid(2)
#include <us_helper.h> // for TRACE()

/*
	This demo explored gettid() and getpid() issues on linux
	This demo shows that:
	- for the main thread (the one going from the main function) the thread id
	is the same as the process id.
	- for other threads the process id is the same but the thread id is different.
	- pthread_self() is different than all of the above (some opaque value).
	- there is no wrapper function in libc for gettid. I use a wrapper
	here that I defined myself in <us_helper.hh>. Check it out. You can either
	use that or use some framework which gives you wrappers.

EXTRA_LIBS=-lpthread
*/

void report(void) {
	TRACE("gettid() returned %d",gettid());
	TRACE("getpid() returned %d",getpid());
	TRACE("pthread_self() returned %lu",pthread_self());
}

void* doit(void*) {
	report();
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	report();
	pthread_t t1,t2;
	CHECK_ZERO(pthread_create(&t1,NULL,doit,NULL));
	CHECK_ZERO(pthread_create(&t2,NULL,doit,NULL));
	CHECK_ZERO(pthread_join(t1,NULL));
	CHECK_ZERO(pthread_join(t2,NULL));
	return EXIT_SUCCESS;
}
