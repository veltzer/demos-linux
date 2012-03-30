#include <pthread.h> // for pthread_create(3), pthread_join(3)

#include "us_helper.hh" // for CHECK_*, TRACE

/*
 * This is a demo of thread local storage using gcc __thread storage modifier.
 *
 * 		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */

__thread int myid;

void* worker(void* arg) {
	int* pint=(int*)arg;
	myid=*pint;
	delete pint;
	// now lets pull our id
	TRACE("my id is %d\n",myid);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	TRACE("start");
	const unsigned int num=4;
	pthread_t threads[num];
	for (unsigned int i = 0; i < num; i++) {
		int* p=new int(i);
		TRACE("allocated %p",p);
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, p));
	}
	TRACE("created threads, now joining...");
	for (unsigned int i = 0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i],NULL));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
