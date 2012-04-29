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

#include<firstinclude.h>
#include<pthread.h> // for pthread_create(3), pthread_join(3), pthread_t
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include<us_helper.h> // for CHECK_ZERO()

/*
* EXTRA_LIBS=-lpthread
*/

void* PrintHello(void *threadid) {
	int *id_ptr, taskid;
	id_ptr = (int *) threadid;
	taskid = *id_ptr;
	printf("Thread %d says hello\n",taskid);
	return (void *)(taskid*2);
	// another option would be to call
	//pthread_exit((void *)taskid);
}

int main(int argc,char** argv,char** envp) {
	const int NUM_THREADS=8;
	pthread_t threads[NUM_THREADS];
	int t[NUM_THREADS];
	// init all values to be passed to threads
	for(int i=0;i<NUM_THREADS;i++) {
		t[i]=i;
	}
	// start all threads giving each it's input
	for(int i=0;i<NUM_THREADS;i++) {
		CHECK_ZERO(pthread_create(&threads[i],NULL,PrintHello,(void *)&t[i]));
	}
	// wait for all threads to finish, order does not matter
	for(int i=0;i<NUM_THREADS;i++) {
		void* retval;
		CHECK_ZERO(pthread_join(threads[i],&retval));
		printf("thread %d returned value %d\n",i,(int)retval);
	}
	return EXIT_SUCCESS;
}
