#include <stdio.h> // for printf(3)
#include <aio.h> // for aio_read(3), aio_suspend(3), aio_return(3)
#include <strings.h> // for bzero(3)
#include <stdlib.h> // for malloc(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)

#include "us_helper.hh" // our own helper

/*
 * This example explores the performance of the write system call...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lrt
 */
int main(int argc, char **argv, char **envp) {
	const int BUFSIZE=1024;
	int fd;
	CHECK_NOT_M1(fd=open("/dev/null",O_RDWR));
	CHECK_NOT_M1(close(fd));
	return(0);
}
