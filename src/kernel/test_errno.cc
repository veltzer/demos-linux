#include <stdio.h> // for printf(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for close(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <errno.h> // for errno(3)

#include "us_helper.hh" // our own helper for user space

/*
 * This is an example which explores how user space experiences errors coming from the kernel.
 *
 * Conclusions:
 * - If kernel returns a negative value (like most kernel code returns when there are errors)
 *	then user space will get -1 as a return value from the kernel function and errno will be
 *	set to the ** positive ** value of the error.
 * - If kernel returns a value which is greater or equal to 0 then user space will get the exact
 *	value that kernel returned and errno for the thread doing the call will not change.
 * - Please recall that errno is a PER THREAD variable.
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/drv_errno";
	// file descriptor
	int d;
	// hold results of syscalls and errnos
	int res, myerrno;
	// default value for errno before we start
	const int def_errno=2005;

	printf("Starting, errno at start is %d\n",def_errno);
	CHECK_NOT_M1(d = open(filename, O_RDWR));

	for (int i = -10; i < 10; i++) {
		errno = def_errno;
		res = ioctl(d, 0, i);
		myerrno = errno;
		printf("kernel returned %d, I got %d and errno is %d\n", i, res, myerrno);
	}
	CHECK_NOT_M1(close(d));
	return(0);
}
