#include<unistd.h> // for sleep(3)
#include<pthread.h> // for pthread_mutex_init(3), pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_destroy(3), pthread_create(3), pthread_join(3)

#include"us_helper.hh"

/*
 * This demos pthread mutex usage.
 * Note that these are actually futexes.
 *
 *	Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */

static pthread_mutex_t mylock;

static void *worker(void *p) {
	unsigned int id=*(unsigned int*)p;
	int counter=0;
	while (counter<10) {
		TRACE("%d before lock",id);
		CHECK_ZERO(pthread_mutex_lock(&mylock));
		sleep(1);
		// this simulates a bug. see what this thread is doing using
		// strace on the process or thread id.
		if(id==1 && counter==10) {
			sleep(1000);
		}
		CHECK_ZERO(pthread_mutex_unlock(&mylock));
		counter++;
		TRACE("%d after lock",id);
		sleep(1);
	}
	return(NULL);
}


int main(int argc, char** argv, char** envp) {
	// first initialize the lock (no need for sharing between processes which
	// is the reason for the 0 in the second argument...)
	TRACE("initializing the lock...");
	CHECK_ZERO(pthread_mutex_init(&mylock,NULL));
	const unsigned int num = 2;
	pthread_t threads[num];
	unsigned int ids[num];
	void* rets[num];
	TRACE("starting threads...");
	for (unsigned int i = 0; i < num; i++) {
		ids[i] = i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("finished creating threads, joining them...");
	for (unsigned int i = 0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	TRACE("joined all threads, destroying the lock...");
	CHECK_ZERO(pthread_mutex_destroy(&mylock));
	return EXIT_SUCCESS;
}
