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
#include <pthread.h> // for pthread_key_t, pthread_key_create(3), pthread_setspecific(3), pthread_getspecific(3), pthread_create(3), pthread_join(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for CHECK_NOT_M1(), TRACE(), CHECK_ZERO()

/*
* This is a demo of thread local storage using the pthread API.
*
* EXTRA_LINK_FLAGS=-lpthread
*/

pthread_key_t key_myid;

void* worker(void* arg) {
	CHECK_NOT_M1(pthread_setspecific(key_myid,arg));
	// now lets pull our id
	int myid=*(int*)pthread_getspecific(key_myid);
	TRACE("my id is %d\n",myid);
	return NULL;
}

void id_dealloc(void* ptr) {
	TRACE("deleting %p",ptr);
	int* p=(int*)ptr;
	delete p;
}

int main(int argc,char** argv,char** envp) {
	TRACE("start");
	CHECK_NOT_M1(pthread_key_create(&key_myid,id_dealloc));
	const unsigned int num=4;
	pthread_t threads[num];
	for(unsigned int i=0;i<num;i++) {
		int* p=new int(i);
		TRACE("allocated %p",p);
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, p));
	}
	TRACE("created threads, now joining...");
	for (unsigned int i=0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i],NULL));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
