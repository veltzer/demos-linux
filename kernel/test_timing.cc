#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      A test that runs ioctl 0 with no data, sleeps for 1 second and then
 *      another ioctl 1 with no data.
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;

	printf("Starting\n");
	SCIE(d = open(filename, O_RDWR), "open");
	SCIE(ioctl(d, 0, NULL), "ioctl");
	// sleep for 1 second to allow us to see the results
	sleep(1);
	SCIE(ioctl(d, 1, NULL), "ioctl");
	SCIE(close(d), "close");
	return(0);
}
