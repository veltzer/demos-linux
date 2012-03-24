#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <unistd.h> // for close(2)

#include "us_helper.hh"

/*
 * This is a demo of how to use the BUG() macro in the kernel to find bugs.
 * Notice that this produces a stack trace in dmesg.
 * Notice also that you can still rmmod your module if you had a bug.
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor to use
	int d;

	CHECK_NOT_M1(d = open(filename, O_RDWR));
	CHECK_NOT_M1(ioctl(d, 0, NULL));
	CHECK_NOT_M1(close(d));
	return(0);
}
