#include <pthread.h> // for pthread_create
#include <unistd.h>  // for sleep

#include "us_helper.hh"

/*
 *      This example creates a zombie thread.
 *      A zombie thread is a thread which is not joined by the parent.
 *      This example shows that once this thread dies it does not
 *      turn into a zombie process as far as the OS is concerned.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void *worker(void *p) {
	DEBUG("starting thread");
	sleep(3);
	DEBUG("ending thread");
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	pthread_t thread;

	DEBUG("main started");
	SCIG(pthread_create(&thread, NULL, worker, NULL), "pthread_create");
	DEBUG("main created thread");
	// we do not join the thread but rather just go to sleep for an hour...
	//SCIG(pthread_join(thread,&ret),"pthread_join");
	// print the status of the threads...
	my_system("ps -L -p %d", getpid());
	// wait for 10 seconds to let the thread end...
	sleep(10);
	// print the status of the threads...
	my_system("ps -L -p %d", getpid());
	DEBUG("main ended");
	return(0);
}
