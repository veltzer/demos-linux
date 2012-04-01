#include<iostream> // for std::cerr, std::endl
#include<signal.h> // for sigqueue(2)
#include<stdio.h> // for perror(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, atoi(3), exit(3)

/*
 * This is a general command line utility to send signals via
 * the sigqueue(2) interface.
 *
 * 			Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	if (argc != 4) {
		std::cerr << argv[0] << ": usage " << argv[0] << " [pid] [sig] [value]" << std::endl;
		return EXIT_FAILURE;
	}
	int pid = atoi(argv[1]);
	int sig = atoi(argv[2]);
	int val = atoi(argv[3]);
	union sigval sval;
	sval.sival_int = val;
	if (sigqueue(pid, sig, sval) == -1) {
		perror("problem calling sigqueue(2)");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
