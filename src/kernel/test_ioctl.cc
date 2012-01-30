#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <unistd.h> // for close(2)

#include "us_helper.hh" // our user helper

/*
 *      A basic test which opens the device, runs one ioctl with 0 or user given
 *      cmd and no data and then closes the device
 *     	
 *     			Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int ioctl_num = 0;

	if (argc > 1) {
		ioctl_num = atoi(argv[1]);
	}
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;
	sc(d = open(filename, O_RDWR));
	sc(ioctl(d, ioctl_num, NULL));
	sc(close(d));
	return(0);
}
