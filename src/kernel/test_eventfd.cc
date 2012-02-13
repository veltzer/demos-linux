#include <sys/eventfd.h> // for eventfd(2)
#include <stdio.h> // for printf(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for fork(2), close(2)
#include <sys/ioctl.h> // for ioctl(2)

#include "us_helper.hh"

/*
 *      This tests eventfd code in the kernel...
 *
 *      TODO:
 *      call ioctl on the eventfd and get the signal...
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/mod_eventfd";
	// file descriptor
	int fd;

	CHECK_NOT_M1(fd = open(filename, O_RDWR));
	int efd;
	CHECK_NOT_M1(efd = eventfd(0, 0));
	int pid;
	CHECK_NOT_M1(pid = fork());
	if(pid==0) {
		printf("Child\n");
	} else {
		printf("Parent\n");
	}
	CHECK_NOT_M1(close(fd));
	return(0);
}
