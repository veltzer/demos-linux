#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      A basic test which opens the device, runs one ioctl with 0 or user given
 *      cmd and no data and then closes the device
 */
int main(int argc, char **argv, char **envp) {
	int ioctl_num = 0;

	if (argc > 1) {
		ioctl_num = atoi(argv[1]);
	}
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;
	SCIE(d = open(filename, O_RDWR), "open");
	SCIE(ioctl(d, ioctl_num, NULL), "ioctl");
	SCIE(close(d), "close");
	return(0);
}
