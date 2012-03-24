#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep(3)
#include <strings.h> // for bzero(3)

#include "us_helper.hh"

/*
 * This is an example of setting thread stack sizes
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 * - show number of page faults for this application.
 * First run a thread that allocates it's space in advance.
 * Then run a thread that does not.
 * show the info from /proc
 */

void ensure_space(unsigned int size) {
	//int dummy;
	//bzero(&dummy-size/2,size/2);
	char buf[500000];
	bzero(buf,500000);
}

void *worker(void *p) {
	int num = *(int *)p;
	ensure_space((num+1)*100*1024);
	fprintf(stderr, "starting thread %d\n", num);
	sleep(100);
	fprintf(stderr, "ending thread %d\n", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int num = 10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	int ids[num];
	void* rets[num];

	fprintf(stderr, "main starting\n");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		CHECK_ZERO(pthread_attr_init(attrs+i));
		CHECK_ZERO(pthread_attr_setstacksize(attrs+i,(i+1)*1024*1024));
		CHECK_ZERO(pthread_create(threads + i, attrs+i, worker, ids + i));
		CHECK_ZERO(pthread_attr_destroy(attrs+i));
	}
	fprintf(stderr, "main ended creating threads\n");
	for (int i = 0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	fprintf(stderr, "main ended\n");
	return(0);
}
