#include <pthread.h> // for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include <unistd.h> // for sleep(3)

#include "us_helper.hh" // our own helper

/*
 *      This is a standard pthread demo
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void *worker(void *p) {
	int num = *(int *)p;
	TRACE("starting thread %d", num);
	pthread_t t=pthread_self();
	int* pointer=(int*)&t;
	TRACE("pthread_self is %d",*pointer);
	sleep(60);
	TRACE("ending thread %d", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int num = 10;
	pthread_t threads[num];
	int ids[num];
	void      *rets[num];

	TRACE("main starting");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		scg(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("main ended creating threads");
	for (int i = 0; i < num; i++) {
		scg(pthread_join(threads[i], rets + i));
	}
	TRACE("main ended");
	return(0);
}
