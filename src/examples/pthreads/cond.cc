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
#include<stdio.h> // for printf(3)
#include<pthread.h> // for pthread_mutex_t, pthread_cond_t, pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_cond_wait(3), pthread_create(3), pthread_join(3), pthread_t
#include<us_helper.h> //

/*
* This is an example that shows how to use pthread conditions.
* Remmember that in order to use these you must protect the entire area where you are checking the condition
* using a mutex. And since conditions do not come with mutexes you must create one yourself.
*
* EXTRA_LIBS=-lpthread
*/

pthread_mutex_t count_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond=PTHREAD_COND_INITIALIZER;

int count=0;
const int COUNT_DONE=10;
const int COUNT_HALT1=3;
const int COUNT_HALT2=6;

void* functionCount1(void*) {
	while(true) {
		pthread_mutex_lock( &condition_mutex );
		while( count >= COUNT_HALT1 && count <= COUNT_HALT2 ) {
			pthread_cond_wait( &condition_cond, &condition_mutex );
		}
		pthread_mutex_unlock( &condition_mutex );
		pthread_mutex_lock( &count_mutex );
		count++;
		printf("Counter value functionCount1: %d\n",count);
		pthread_mutex_unlock( &count_mutex );
		if(count >= COUNT_DONE) return(NULL);
	}
}

void *functionCount2(void*) {
	while(true) {
		pthread_mutex_lock( &condition_mutex );
		if( count < COUNT_HALT1 || count > COUNT_HALT2 ) {
			pthread_cond_signal( &condition_cond );
		}
		pthread_mutex_unlock( &condition_mutex );
		pthread_mutex_lock( &count_mutex );
		count++;
		printf("Counter value functionCount2: %d\n",count);
		pthread_mutex_unlock( &count_mutex );
		if(count >= COUNT_DONE) return(NULL);
	}
}
int main(int argc,char** argv,char** envp) {
	pthread_t thread1,thread2;
	CHECK_ZERO(pthread_create(&thread1,NULL,&functionCount1,NULL));
	CHECK_ZERO(pthread_create(&thread2,NULL,&functionCount2,NULL));
	CHECK_ZERO(pthread_join(thread1,NULL));
	CHECK_ZERO(pthread_join(thread2,NULL));
	return EXIT_SUCCESS;
}
