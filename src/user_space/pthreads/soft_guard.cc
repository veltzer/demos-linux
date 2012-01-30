#include <pthread.h> // for pthread_create, pthread_join
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for exit(3)
#include <unistd.h> // for syscall(2)
#include <sys/types.h> // for gettid(2) 
#include <sys/syscall.h> // for syscall(2)
#include <strings.h> // for bzero(3)

#include "us_helper.hh"

/*
 * This test shows how to test for bad stack state (distance from
 * end of the stack is too short...)
 *
 * 				Mark Veltzer
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
	return 0;
}
