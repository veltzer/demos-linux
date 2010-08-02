#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static int counterUSR1 = 0;
static int counterUSR2 = 0;
static int flag = 0;

static void SignalHandlerUSR1(int sig) {
	counterUSR1++;
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " starting" << std::endl;
}


static void SignalHandlerUSR2(int sig) {
	counterUSR2++;
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " starting" << std::endl;
	// reverse the flag
	if (flag == 0) {
		flag = 1;
	} else {
		flag = 0;
	}
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " setting flag to " << flag << std::endl;
	if (siginterrupt(SIGUSR1, flag) == -1) {
		perror("problem with calling siginterrupt(2)");
		exit(1);
	}
}


int main(int argc, char **argv, char **envp) {
	// set up the signal handler (only need to do this once)
	if (signal(SIGUSR1, SignalHandlerUSR1) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}
	if (signal(SIGUSR2, SignalHandlerUSR2) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}
	std::cerr << "main: set up the sig handler, lets start" << std::endl;
	// This is a non busy wait loop which only wakes up when there
	// are signals
	const int size = 256;
	char buf[size];
	while (1) {
		std::cerr << "main: before read" << std::endl;
		if (read(0, buf, size) == -1) {
			perror("main: error in read(2)");
		}
		std::cerr << "main: after read" << std::endl;
	}
	return(0);
}
