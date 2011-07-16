#include <unistd.h> // for sleep(3)
#include <pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_create(3), pthread_join(3), pthread_mutex_init(3)

#include "us_helper.hh"

/*
 * This demos mutex usage.
 *
 *                      Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */

static pthread_mutex_t mylock;

static void *worker(void *p) {
	int id=*(int*)p;
	int counter=0;
	while (true) {
		TRACE("before lock");
		scig(pthread_mutex_lock(&mylock), "pthread_spin_lock");
		sleep(1);
		// this simulates a bug. see what this thread is doing using
		// strace on the process or thread id.
		if(id==1 && counter==10) {
			sleep(1000);
		}
		scig(pthread_mutex_unlock(&mylock), "pthread_spin_unlock");
		counter++;
		TRACE("after lock");
		sleep(1);
	}
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	// first initialize the lock (no need for sharing between processes which
	// is the reason for the 0 in the second argument...)
	TRACE("initializing the lock...");
	scig(pthread_mutex_init(&mylock,NULL), "pthread_spin_init");
	const int num = 2;
	pthread_t threads[num];
	int ids[num];
	void      *rets[num];
	TRACE("starting threads...");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		scig(pthread_create(threads + i, NULL, worker, ids + i), "pthread_create");
	}
	TRACE("finished creating threads, joining them...");
	for (int i = 0; i < num; i++) {
		scig(pthread_join(threads[i], rets + i), "pthread_join");
	}
	TRACE("joined all threads, destroying the lock...");
	scig(pthread_mutex_destroy(&mylock), "pthread_spin_destroy");
	return(0);
}
