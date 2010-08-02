#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "us_helper.hh"

/*
 *      This application has two threads:
 *              - one running like crazy doing ioctls.
 *              - one asking to close the driver (which halts).
 *              - main asking for input and releases the close process...
 *
 * EXTRA_LIBS=-lpthread
 */

/*
 *      What do we learn from this?
 *      - closing a file descriptor only schedules release to be called in the kernel.
 *      - this release will be called once any ioctls on the current file descriptor end.
 *      - any new operations on this file descriptor are not allowed (bad file descriptor).
 */

// file descriptor
int d, d2;

void *function_crazy(void *p) {
	bool over = false;
	int counter = 0;
	bool err = false;

	while (!over) {
		counter++;
		// ioctl to do nothing...
		int res = ioctl(d, 0, NULL);
		if (res == -1) {
			err = true;
			//perror("ERROR from crazy thread...");
			//over=true;
		} else {
		}
		if (counter % 10000 == 0) {
			char c;
			if (err) {
				c = 'E';
			} else {
				c = '.';
			}
			fprintf(stdout, "%c", c);
			fflush(stdout);
			err = false;
		}
	}
	return(NULL);
}


void *function_long(void *p) {
	SCIE(ioctl(d, 1, NULL), "long ioctl 1");
	SCIE(ioctl(d, 1, NULL), "long ioctl 2");
	return(NULL);
}


void *function_long2(void *p) {
	SCIE(ioctl(d2, 2, NULL), "very long ioctl");
	return(NULL);
}


void *function_close(void *p) {
	fprintf(stderr, "close thread started and going to sleep for two seconds\n");
	sleep(2);
	fprintf(stderr, "close thread trying to close handle...\n");
	SCIE(close(d), "close file");
	//SCIE(ioctl(d,0,NULL),"short ioctl");
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";

	SCIE(d = open(filename, O_RDWR), "open first");
	SCIE(d2 = open(filename, O_RDWR), "open second");

	//pthread_t thread_crazy;
	//SCIG(pthread_create(&thread_crazy,NULL,function_crazy,NULL),"pthread crazy created");
	pthread_t thread_long;
	SCIG(pthread_create(&thread_long, NULL, function_long, NULL), "pthread long created");
	pthread_t thread_long2;
	SCIG(pthread_create(&thread_long2, NULL, function_long2, NULL), "pthread long created");
	pthread_t thread_close;
	SCIG(pthread_create(&thread_close, NULL, function_close, NULL), "pthread close created");

	//waitkey("press any key to wake the close thread up");
	//SCIE(ioctl(d,5,NULL),"waking the close thread");

	//SCIG(pthread_join(thread_crazy,NULL),"join");
	SCIG(pthread_join(thread_long, NULL), "join");
	SCIG(pthread_join(thread_long2, NULL), "join");
	SCIG(pthread_join(thread_close, NULL), "join");
	return(0);
}
