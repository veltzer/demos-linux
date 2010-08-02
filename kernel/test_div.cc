#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

#include "div_buffer.h"

/*
 *      A basic test which opens the device, runs one ioctl with 0 or user given
 *      cmd and no data and then closes the device
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;
	// size of strings
	const unsigned int len = 256;
	// two strings to get the data
	char s1[len];
	char s2[len];

	SCIE(d = open(filename, O_RDWR), "open");
	while (true) {
		// read two numbers from the user
		printf("Please enter a number 1:");
		fflush(stdout);
		if (fgets(s1, len, stdin) != s1) {
			perror("problem with fgets");
			exit(1);
		}
		printf("Please enter a number 2:");
		fflush(stdout);
		if (fgets(s2, len, stdin) != s2) {
			perror("problem with fgets");
			exit(1);
		}
		buffer b;
		b.u1 = atoi(s1);
		b.u2 = atoi(s2);
		b.d1 = atoi(s1);
		b.d2 = atoi(s2);
		klog_clear();
		scie(ioctl(d, 0, &b), "ioctl to div");
		klog_show();
		printf("unsigned long long results:\n");
		printf("div is %llu - should be %llu\n", b.udiv, b.u1 / b.u2);
		printf("mul is %llu - should be %llu\n", b.umul, b.u1 * b.u2);
		printf("add is %llu - should be %llu\n", b.uadd, b.u1 + b.u2);
		printf("sub is %llu - should be %llu\n", b.usub, b.u1 - b.u2);
		printf("long long results:\n");
		printf("div is %lld - should be %lld\n", b.ddiv, b.u1 / b.u2);
		printf("mul is %lld - should be %lld\n", b.dmul, b.u1 * b.u2);
		printf("add is %lld - should be %lld\n", b.dadd, b.u1 + b.u2);
		printf("sub is %lld - should be %lld\n", b.dsub, b.u1 - b.u2);
	}
	SCIE(close(d), "close");
	return(0);
}
