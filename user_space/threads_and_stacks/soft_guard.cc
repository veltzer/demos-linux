//#define _GNU_SOURCE
#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for exit(3)
#include<unistd.h> // for syscall(2)
#include<sys/types.h> // for gettid(2) 
#include<sys/syscall.h> // for syscall(2)
#include<strings.h> // for bzero(3)

/*
	This test shows how to test for bad stack state (distance from
	end of the stack is too short...)
 * EXTRA_LIBS=-lpthread
*/

typedef struct big_struct {
	char data[7000000];
} my_big_struct;

void check_stack() {
	pthread_attr_t at;
	int res=pthread_getattr_np(pthread_self(),&at);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
	void* stackaddr;
	size_t stacksize;
	res=pthread_attr_getstack(&at,&stackaddr,&stacksize);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
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
	res=pthread_attr_destroy(&at);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
}

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
	int res;
	res=pthread_create(&t,NULL,doit,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
	res=pthread_join(t,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_join");
		exit(-1);
	}
	return 0;
}
