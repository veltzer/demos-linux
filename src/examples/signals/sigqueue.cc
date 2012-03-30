#include<iostream>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

/*
 * This is a general command line utility to send signals via
 * the sigqueue(2) interface.
 *
 * 			Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	if (argc != 4) {
		std::cerr << argv[0] << ": usage " << argv[0] << " [pid] [sig] [value]" << std::endl;
		return(-1);
	}
	int pid = atoi(argv[1]);
	int sig = atoi(argv[2]);
	int val = atoi(argv[3]);
	union sigval sval;
	sval.sival_int = val;
	if (sigqueue(pid, sig, sval) == -1) {
		perror("problem calling sigqueue(2)");
		exit(1);
	}
	return EXIT_SUCCESS;
}
