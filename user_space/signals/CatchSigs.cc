#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static int counter = 0;

static void SignalHandler(int sig) {
	// before we throw an exception or do a longjmp we need to unblock
	// the signal or the kernel will think we are still in the signal handler
	counter++;
	std::cerr << "handler: " << counter << " starting sig handler for signal [" << strsignal(sig) << "]" << std::endl;
	std::cerr << "handler: " << counter << " going to sleep" << std::endl;
	sleep(10);
	std::cerr << "handler: " << counter << " wakeup" << std::endl;

	std::cerr << "handler: " << counter << " ending sig handler for signal " << sig << std::endl;
}


int main(int argc, char **argv, char **envp) {
	// set up the signal handler (only need to do this once)
	if (signal(SIGUSR1, SignalHandler) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}
	if (signal(SIGUSR2, SignalHandler) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}

	/*
	 * struct sigaction sa;
	 * if(sigaction(SIGUSR1,sa,NULL)==-1) {
	 *	perror("problem with calling sigaction(2)");
	 *	exit(1);
	 * }
	 * if(sigaction(SIGUSR2,sa,NULL)==-1) {
	 *	perror("problem with calling sigaction(2)");
	 *	exit(1);
	 * }
	 */
	std::cerr << "main: set up the sig handler, lets start" << std::endl;
	// This is a non busy wait loop which only wakes up when there
	// are signals
	while (1) {
		int ret = pause();
		std::cerr << "main: wakeup with ret " << ret << std::endl;
	}
	return(0);
}
