#include<stdio.h> // for printf(3)
#include<pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_init(3), pthread_mutex_destory(3)

#include"us_helper.hh" // for CHECK_ZERO

/*
 * This example creates a pthread_mutex which is a futex, grabs it and releases
 * it. Run it with strace to see that it does not call any system call.
 *
 * Notes:
 * - there is no syscall involved with this futex AT ALL! This is because this futex is private
 *	and therefore not robust. Futexes can be robust in which case you need to tell that at creation
 *	which will notify the kernel about this futex.
 *	by default futexes are private.
 *	This means that even creation of futexes is cheap.
 *
 * Problem:
 * - even if I create a process shared mutex the pthread library calls no syscall at mutex_init time!!!
 *	How can this be if the futex is robust?!? investigate...
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO: make this example strace itself...
 */

int main(int argc, char **argv, char **envp) {
	printf("main started\n");
	// this is the default type of locked (the "FAST" kind...) using the
	// special initialisation syntax...
	//pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
	
	// this is creation of pthread using the init function...
	//pthread_mutex_t mutex;
	//CHECK_ZERO(pthread_mutex_init(&mutex,NULL));

	// this is a shared mutex version
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
	CHECK_ZERO(pthread_mutexattr_init(&attr));
	CHECK_ZERO(pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED));
	CHECK_ZERO(pthread_mutex_init(&mutex,&attr));

	CHECK_ZERO(pthread_mutex_lock(&mutex));
	printf("mutex address is %p\n",&mutex);
	CHECK_ZERO(pthread_mutex_unlock(&mutex));
	CHECK_ZERO(pthread_mutex_destroy(&mutex));
	printf("main ended\n");
	return EXIT_SUCCESS;
}
