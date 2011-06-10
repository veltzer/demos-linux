#include <stdio.h> // for fprintf(3)
#include <sys/ioctl.h> // for ioctl(2)
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)

#include "us_helper.hh"

/*
 *      This is a demo of how to put a thread to sleep and wake it up
 *      from another thread...
 *
 * EXTRA_LIBS=-lpthread -lcpufreq
 */

// file descriptor
int d;

void *wait_function(void *p) {
	fprintf(stderr,"wait thread started\n");
	ticks_t t1 = getticks();
	// wait thread going to sleep
	SC(ioctl(d, 3, 1000));
	ticks_t t2 = getticks();
	fprintf(stderr,"took %d micros\n",get_mic_diff(t1, t2));
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";

	SC(d = open(filename, O_RDWR));
	if (argc == 1) {
		SC(ioctl(d, 0, NULL));
	}
	pthread_t thread_wait1, thread_wait2;
	SCIG(pthread_create(&thread_wait1, NULL, wait_function, NULL), "pthread_created");
	SCIG(pthread_create(&thread_wait2, NULL, wait_function, NULL), "pthread_created");
	waitkey("press any key to wake the thread up");
	// waking the thread
	SC(ioctl(d, 5, NULL));
	SCIG(pthread_join(thread_wait1, NULL), "join");
	SCIG(pthread_join(thread_wait2, NULL), "join");
	SC(close(d));
	return(0);
}
