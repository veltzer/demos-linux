#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "shared.h"

#include "us_helper.hh"

/*
 *      A test that runs ioctl 0 with no data, sleeps for 1 second and then
 *      another ioctl 1 with no data.
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/mod_timing";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_timing");
	my_system("sudo insmod ./mod_timing.ko");
	my_system("sudo chmod 666 %s",filename);
	// file descriptor
	int d;

	printf("Starting\n");
	sc(d = open(filename, O_RDWR));
	sc(ioctl(d, IOCTL_TIMING_CLOCK, NULL));
	// sleep for 1 second to allow us to see the results
	//sleep(1);
	sc(ioctl(d, IOCTL_TIMING_TSC, NULL));
	
	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("doing %d syscalls\n",loop);
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0;i < loop;i++) {
		sc(ioctl(d, IOCTL_TIMING_EMPTY, NULL));
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	sc(close(d));
	return(0);
}
