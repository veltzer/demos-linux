#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "us_helper.hh"

/*
 *      This is a demo which shows that perror, errno etc all work on a thread
 *      by thread level...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void *worker1(void *p) {
	fprintf(stderr, "worker1 starting thread\n");
	fprintf(stderr, "test 1: with no error\n");
	int err = errno;
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "test 2: doing something illegal\n");
	open("/etc/nonexistant", O_RDWR);
	err = errno;
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "test 3: setting error myself\n");
	errno = 3;
	err = 3;
	perror("msg from perror");
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "worker1 ending thread\n");
	return(NULL);
}


void *worker2(void *p) {
	fprintf(stderr, "worker2 starting thread\n");
	fprintf(stderr, "errno is %d\n", errno);
	perror("printing perror");
	fprintf(stderr, "worker2 ending thread\n");
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	pthread_t thread1;
	pthread_t thread2;
	const int num = 2;
	int ids[num];
	void      *rets[num];

	fprintf(stderr, "main starting\n");
	SCIG(pthread_create(&thread1, NULL, worker1, ids + 0), "pthread_create");
	SCIG(pthread_create(&thread2, NULL, worker2, ids + 1), "pthread_create");
	fprintf(stderr, "main ended creating threads\n");
	SCIG(pthread_join(thread1, rets + 0), "pthread_join");
	SCIG(pthread_join(thread1, rets + 1), "pthread_join");
	fprintf(stderr, "main ended\n");
	return(0);
}
