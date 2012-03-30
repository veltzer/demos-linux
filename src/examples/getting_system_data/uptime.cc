#include<stdio.h> // for printf(3)
#include<string.h> // for strtok_r(3)
#include<sys/types.h> // for open(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for read(2), close(2)
#include<stdlib.h> // for stof(3)

#include"us_helper.hh"

/*
 * This demo shows how to get the uptime of the machine...
 * This is not very suitable for very delicate and precise timings but still.
 *
 *		Mark Veltzer
 */
void uptime(float *time1, float *time2) {
	// null the pointers
	*time1 = 0;
	*time2 = 0;
	// read the data from the /proc/uptime virtual file...
	const char* filename = "/proc/uptime";
	const unsigned int size = 256;
	char buf[size];
	int d, res;
	CHECK_NOT_M1(d = open(filename, O_RDONLY));
	CHECK_NOT_M1(res = read(d, buf, size));
	char *saveptr;
	char *ptr = strtok_r(buf, " ", &saveptr);
	*time1 = atof(ptr);
	ptr = strtok_r(NULL, " ", &saveptr);
	*time2 = atof(ptr);
	CHECK_NOT_M1(close(d));
}


int main(int argc, char **argv, char **envp) {
	float time1, time2;
	uptime(&time1, &time2);
	printf("time1 is %f\n", time1);
	printf("time2 is %f\n", time2);
	return EXIT_SUCCESS;
}
