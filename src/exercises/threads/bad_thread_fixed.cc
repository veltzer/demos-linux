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
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for sleep(3)
#include <stdlib.h>	// for EXIT_SUCCESS, rand(3), srand(3)
#include <us_helper.h>	// for CHECK_ZERO()
#include <time.h>	// for time(2)

/*
 * EXTRA_LINK_FLAGS=-lpthread
 */

void* PrintHello(void *threadid) {
	int *id_ptr, taskid;
	id_ptr=(int *) threadid;
	int stime;
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	taskid=*id_ptr;
	printf("Thread %d says hello\n", taskid);
	return threadid;
}

int main(int argc, char** argv, char** envp) {
	const int NUM_THREADS=8;
	pthread_t threads[NUM_THREADS];
	int t[NUM_THREADS];
	int retval[NUM_THREADS];
	int stime;
	srand(time(NULL));
	for(int i=0; i<NUM_THREADS; i++) {
		t[i]=i;
		CHECK_ZERO(pthread_create(&threads[i], NULL, PrintHello, (void *) &t[i]));
	}
	for(int i=0; i<NUM_THREADS; i++) {
		stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
		sleep(stime);
		void* cretval;
		CHECK_ZERO(pthread_join(threads[i], &cretval));
		retval[i]=(int)cretval;
		printf("thread returned value %d\n", retval[i]);
	}
	return EXIT_SUCCESS;
}
