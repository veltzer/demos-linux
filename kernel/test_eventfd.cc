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

	sc(fd = open(filename, O_RDWR));
	int efd;
	sc(efd = eventfd(0, 0));
	int pid;
	sc(pid = fork());
	switch (pid) {
		case 0:
			printf("Child\n");

		default:
			printf("Parent\n");
	}
	sc(close(fd));
	return(0);
}
