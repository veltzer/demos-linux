#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      This tests the get_free_pages driver
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file handle
	int d;

	sc(d = open(filename, O_RDWR));
	sc(close(d));
	return(0);
}
