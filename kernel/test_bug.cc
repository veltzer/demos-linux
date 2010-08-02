#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>

#include "us_helper.hh"

/*
 *      This is a demo of how to use the BUG() macro in the kernel to find bugs...
 *      Notice that this produces a stack trace in dmesg...
 *      Notice also that you can still rmmod your module if you had a bug...
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor to use
	int d;

	SCIE(d = open(filename, O_RDWR), "open");
	SCIE(ioctl(d, 0, NULL), "activate bug");
	SCIE(close(d), "close file");
	return(0);
}
