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
	// hold results of syscalls and errnos
	int res, myerrno;

	printf("Starting\n");
	SCIE(d = open(filename, O_RDWR), "open");

	for (int i = -1000; i < 1000; i++) {
		errno = 2005;
		res = ioctl(d, 0, i);
		myerrno = errno;
		printf("code is %d, res is %d, errno is %d\n", i, res, myerrno);
	}
	SCIE(close(d), "close");
	return(0);
}
