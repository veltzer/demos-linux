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

	SC(d = open(filename, O_RDWR));
	int efd;
	SC(efd = eventfd(0, 0));
	int pid;
	SC(pid = fork());
	switch (pid) {
		case 0:
			printf("Child\n");

		default:
			printf("Parent\n");
	}
	SC(close(d));
	return(0);
}
