#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)

/*
 * This demo demostrates how to cause a thread that is stuck in a long system call to
 * break out of it. The idea is to generate a signal and to define that signal as an
 * interrupt signal. This will not cause the system call to be restarted but rather
 * the system call (in this examples case read(2)) will return with an error(-1) and
 * the error code will be -EINTR. On receiving this the main thread will politely exit
 * (throw an exception ?!?).
 *
 * 			Mark Veltzer
 *
 * TODO:
 * - turn this into a pure C example.
 */

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
	std::cerr << "send signals to me using:" << std::endl;
	std::cerr << "kill -s SIGUSR1 " << getpid() << std::endl;
	// This is a non busy wait loop which only wakes up when there
	// are signals
	const int size = 256;
	char buf[size];
	while (true) {
		std::cerr << "main: before read" << std::endl;
		if (read(fileno(stdin), buf, size) == -1) {
			perror("main: error in read(2)");
		}
		std::cerr << "main: after read" << std::endl;
	}
	return EXIT_SUCCESS;
}
