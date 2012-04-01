#include<signal.h> // for sigqueue(2), SIGRTMIN, SIGRTMIN
#include<stdio.h> // for perror(3), fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
 * This is a general command line utility to send signals via
 * the sigqueue(2) interface.
 *
 * 			Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	if (argc != 4) {
		fprintf(stderr,"usage: %s [pid] [sig] [value]\n",argv[0]);
		fprintf(stderr,"Info for real time signals follows:\n");
		fprintf(stderr,"SIGRTMIN is %d\n",SIGRTMIN);
		fprintf(stderr,"SIGRTMAX is %d\n",SIGRTMAX);
		return EXIT_FAILURE;
	}
	int pid = atoi(argv[1]);
	int sig = atoi(argv[2]);
	int val = atoi(argv[3]);
	union sigval sval;
	sval.sival_int=val;
	CHECK_NOT_M1(sigqueue(pid,sig,sval));
	return EXIT_SUCCESS;
}
