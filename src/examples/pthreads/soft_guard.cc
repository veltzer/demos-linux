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
#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for exit(3)
#include<unistd.h> // for syscall(2)
#include<sys/types.h> // for gettid(2)
#include<sys/syscall.h> // for syscall(2)
#include<strings.h> // for bzero(3)

#include<us_helper.h>

/*
* This test shows how to test for bad stack state (distance from
* end of the stack is too short...)
*
* EXTRA_LIBS=-lpthread
*/

void check_stack() {
	pthread_attr_t at;
	CHECK_ZERO(pthread_getattr_np(pthread_self(),&at));
	void* stackaddr;
	size_t stacksize;
	CHECK_ZERO(pthread_attr_getstack(&at,&stackaddr,&stacksize));
	void* myaddr=&at;
	unsigned int diff=(char*)myaddr-(char*)stackaddr;
	if(diff<stacksize*0.2) {
		fprintf(stderr,"You are too close to the stack edge...\n");
		fprintf(stderr,"======================================\n");
		fprintf(stderr,"stacksize is %d\n",stacksize);
		fprintf(stderr,"stackaddr %p\n",stackaddr);
		fprintf(stderr,"myaddr %p\n",myaddr);
		fprintf(stderr,"diff %u\n",diff);
		for(int i=10;i>0;i--) {
			fprintf(stderr,"I will die in %2d seconds...\r",i);
			sleep(1);
		}
		fprintf(stderr,"\n");
		exit(-1);
	}
	CHECK_ZERO(pthread_attr_destroy(&at));
}

typedef struct big_struct {
	char data[7000000];
} my_big_struct;

void call_big() {
	my_big_struct biggie;
	bzero(&biggie,sizeof(my_big_struct));
	check_stack();
	printf("disregard this print %p\n",&biggie);
}

void* doit(void*) {
	call_big();
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	pthread_t t;
	CHECK_ZERO(pthread_create(&t,NULL,doit,NULL));
	CHECK_ZERO(pthread_join(t,NULL));
	return EXIT_SUCCESS;
}
