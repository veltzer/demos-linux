#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "us_helper.hh"

/*
 *      This is a demo for the sched_yield(2) function.
 *      The demostration that this program works can be seen in it's output
 *      in which the prints of the two threads are precisly interwoven which
 *      means that they yield the CPU one to the other...
 *
 *                      Mark Veltzer
 * EXTRA_LIBS=-lpthread
 */
void *worker(void *p) {
	while (true) {
		TRACE("im here");
		scie(sched_yield(), "sched_yield");
	}
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int num = 2;
	pthread_t threads[num];
	int ids[num];
	void      *rets[num];

	fprintf(stderr, "main starting\n");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		SCIG(pthread_create(threads + i, NULL, worker, ids + i), "pthread_create");
	}
	fprintf(stderr, "main ended creating threads\n");
	for (int i = 0; i < num; i++) {
		SCIG(pthread_join(threads[i], rets + i), "pthread_join");
	}
	fprintf(stderr, "main ended\n");
	return(0);
}
