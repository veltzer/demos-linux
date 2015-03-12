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
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <trace_utils.h>// for TRACE()
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()

/*
 * This is a demo of thread local storage using gcc __thread storage modifier.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

__thread int myid;

void* worker(void* arg) {
	int* pint=(int*)arg;
	myid=*pint;
	free(pint);
	// now lets pull our id
	TRACE("myid is %d\n", myid);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	unsigned int i;
	const unsigned int num=4;
	pthread_t threads[num];
	TRACE("start");
	for(i=0; i<num; i++) {
		int* p=(int*)malloc(sizeof(int));
		TRACE("allocated %p", p);
		CHECK_ZERO_ERRNO(pthread_create(threads + i, NULL, worker, p));
	}
	TRACE("created threads, now joining...");
	for(i=0; i<num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], NULL));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
