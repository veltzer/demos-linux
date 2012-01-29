#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <unistd.h> // for close(2), sleep(3), usleep(3)
#include <stdio.h> // for printf(3)

#include "shared.h" // for ioctl numbers

#include "us_helper.hh" // our user helper

/*
 * This is a simple test to see the use count of kernel modules
 *     	
 *     			Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	const char *filename = "/dev/mod_use_count";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_use_count");
	my_system("sudo insmod ./mod_use_count.ko");
	my_system("sudo chmod 666 %s",filename);
	printf("run something like watch --interval=0.2 lsmod\n");
	// file descriptor
	int fd;
	while(true) {
		sc(fd = open(filename, O_RDWR));
		usleep(1000000);
		//sleep(1);
		sc(close(fd));
		usleep(1000000);
		//sleep(1);
	}
	return(0);
}
