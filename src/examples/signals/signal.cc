#include <signal.h> // for signal(2)
#include <stdio.h> // for perror(3)
#include <stdlib.h> // for exit(3)
#include <unistd.h> // for pause(2), getpid(2)
#include <sys/types.h> // for getpid(2)

#include "us_helper.hh" // for TRACE

/*
 * This is a simple example which shows how to do signal handling with the
 * signal(2) syscall. Mind you that this is the old system call and there is a
 * better sigaction(2) syscall at your disposal.
 *
 * Things to note:
 * - the same signal handler can be used for handling more than one signal.
 * - if you send a signal when the signal handler is active then it is not
 *	activated (meaning that the code that you write need not be re-entrant).
 * - the signal is remmember though and will be activated the first thing after
 *	the signal handler is over.
 * - the system does not remmember more than 1 signal. It actually has a bit
 *	mask of waiting signals so each waiting signal is either on or off.
 * - the pause(2) call can be used to block until a signal arrives an provide
 *	you with a non-busy wait loop on signals.
 *
 *		Mark Veltzer
 */

static unsigned int counter = 0;

static void sig_handler(int sig) {
	counter++;
	TRACE("start %d handler, sig is %d",counter,sig);
	TRACE("sleeping for 10 seconds...");
	sleep(10);
	TRACE("end");
}

int main(int argc, char **argv, char **envp) {
	// set up the signal handler (only need to do this once)
	if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}
	if (signal(SIGUSR2, sig_handler) == SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(1);
	}
	TRACE("set up the sig handler");
	TRACE("send me SIGUSR1 using [kill -s SIGUSR1 %d]",getpid());
	TRACE("send me SIGUSR2 using [kill -s SIGUSR2 %d]",getpid());
	// This is a non busy wait loop which only wakes up when there
	// are signals
	while (true) {
		int ret = pause();
		TRACE("wakeup with %d",ret);
	}
	return(0);
}
