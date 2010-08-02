#include <sys/eventfd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      This tests eventfd code in the kernel...
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;

	SCIE(d = open(filename, O_RDWR), "open");
	int efd;
	SCIE(efd = eventfd(0, 0), "open");
	int pid;
	SCIE(pid = fork(), "fork");
	switch (pid) {
	case 0:
		printf("Child\n");

	default:
		printf("Parent\n");
	}
	SCIE(close(d), "close");
	return(0);
}
