#include <stdio.h> // for fprintf(3), fflush(3)
#include <unistd.h> // for sleep(3), close(2)
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <sys/ioctl.h> // for ioctl(2)

#include "shared.h" // for ioctl numbers

#include "us_helper.hh"

/*
 *      This application has two threads:
 *              - one running like crazy doing ioctls.
 *              - one asking to close the driver (which halts).
 *              - main asking for input and releases the close process...
 *
 *      What do we learn from this?
 *      - closing a file descriptor only schedules release to be called in the kernel.
 *      - this release will be called once any ioctls on the current file descriptor end.
 *      - any new operations on this file descriptor are not allowed (bad file descriptor).
 *      - makes it easier to program in the kernel.
 *
 *      		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */

// file descriptor
int fd, fd2;

void *function_empty(void *p) {
	bool over = false;
	int counter = 0;
	int errors=0;
	while (!over) {
		counter++;
		bool err;
		// ioctl to do nothing...
		int res = ioctl(fd, IOCTL_RACE_EMPTY, NULL);
		if (res == -1) {
			err=true;
			errors++;
		} else {
			err=false;
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
		}
		if(errors==100000) {
			over=true;
			fprintf(stdout, "X");
			fflush(stdout);
		}
	}
	return(NULL);
}


void *function_short(void *p) {
	fprintf(stdout, "ss");
	fflush(stdout);
	CHECK_NOT_M1(ioctl(fd, IOCTL_RACE_SLEEP_SHORT, NULL));
	fprintf(stdout, "fs");
	fflush(stdout);
	return(NULL);
}


void *function_long(void *p) {
	fprintf(stdout, "sl");
	fflush(stdout);
	CHECK_NOT_M1(ioctl(fd2, IOCTL_RACE_SLEEP_LONG, NULL));
	fprintf(stdout, "fl");
	fflush(stdout);
	return(NULL);
}


void *function_close(void *p) {
	sleep(2);
	fprintf(stdout,"c");
	fflush(stdout);
	CHECK_NOT_M1(close(fd));
	fprintf(stdout,"C");
	fflush(stdout);
	// this will create an error
	//CHECK_NOT_M1(ioctl(d,IOCTL_RACE_EMPTY,NULL));
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const char *filename = "/dev/mod_ioctl_close_race";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_ioctl_close_race");
	my_system("sudo insmod ./mod_ioctl_close_race.ko");
	my_system("sudo chmod 666 %s",filename);

	CHECK_NOT_M1(fd=open(filename, O_RDWR));
	CHECK_NOT_M1(fd2=open(filename, O_RDWR));

	pthread_t thread_empty,thread_short,thread_long,thread_close;
	CHECK_ZERO(pthread_create(&thread_empty, NULL, function_empty, NULL));
	CHECK_ZERO(pthread_create(&thread_short, NULL, function_short, NULL));
	CHECK_ZERO(pthread_create(&thread_long, NULL, function_long, NULL));
	CHECK_ZERO(pthread_create(&thread_close, NULL, function_close, NULL));
	CHECK_ZERO(pthread_join(thread_empty, NULL));
	CHECK_ZERO(pthread_join(thread_short, NULL));
	CHECK_ZERO(pthread_join(thread_long, NULL));
	CHECK_ZERO(pthread_join(thread_close, NULL));
	//CHECK_NOT_M1(close(fd));
	CHECK_NOT_M1(close(fd2));
	fprintf(stdout,"\nALL DONE\n");
	return(0);
}
