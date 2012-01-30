#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <sys/types.h> // for gettid(2), getpid(2)
#include <unistd.h> // for getpid(2)

#include "us_helper.hh" // for TRACE()

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
 
			Mark Veltzer
EXTRA_LIBS=-lpthread
*/

void report(void) {
	TRACE("gettid() returned %d",gettid());
	TRACE("getpid() returned %d",getpid());
	TRACE("pthread_self() returned %u",(unsigned int)pthread_self());
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
	return 0;
}
