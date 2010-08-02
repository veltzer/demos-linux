#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      This demo shows how to get the uptime of the machine...
 *
 *      This is not very suitable for very delicate and precise timings but still.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
void uptime(float *time1, float *time2) {
	// null the pointers
	*time1 = 0;
	*time2 = 0;
	// read the data from the /proc/uptime virtual file...
	const char         *filename = "/proc/uptime";
	const unsigned int size = 256;
	char buf[size];
	int d, res;
	scie(d = open(filename, O_RDONLY), "open");
	scie(res = read(d, buf, size), "read");
	char *saveptr;
	char *ptr = strtok_r(buf, " ", &saveptr);
	*time1 = atof(ptr);
	ptr = strtok_r(NULL, " ", &saveptr);
	*time2 = atof(ptr);
	scie(close(d), "close file");
}


int main(int argc, char **argv, char **envp) {
	float time1, time2;

	uptime(&time1, &time2);
	printf("time1 is %f\n", time1);
	printf("time2 is %f\n", time2);
	return(0);
}
