#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)

#include"us_helper.hh"

/*
	This demo explored gettid() and getpid() issues on linux
	This demo shows that:
	- for the main thread (the one going from the main function) the thread id
	is the same as the process id.
	- for other threads the process id is the same but the thread id is different.
	- pthread_self() is different than all of the above (some opaque value).
	- there is no wrapper function in libc for gettid. I use a wrapper
	here that I defined myself in "us_helper.hh". Check it out. You can either
	use that or use some framework which gives you wrappers.
 
EXTRA_LIBS=-lpthread
*/

void* doit(void*) {
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	pthread_t t1,t2;
	CHECK_ZERO(pthread_create(&t1,NULL,doit,NULL));
	CHECK_ZERO(pthread_create(&t2,NULL,doit,NULL));
	CHECK_ZERO(pthread_join(t1,NULL));
	CHECK_ZERO(pthread_join(t2,NULL));
	return 0;
}
