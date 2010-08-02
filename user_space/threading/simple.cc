#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "us_helper.hh"

/*
 *      This is a standard pthread demo
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void *worker(void *p) {
	int num = *(int *)p;

	fprintf(stderr, "starting thread %d\n", num);
	fprintf(stderr, "ending thread %d\n", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int num = 10;
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
