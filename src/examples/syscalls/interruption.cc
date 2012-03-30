#include <stdio.h> // for scanf(3), printf(3), fflush(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)
#include <signal.h> // for signal(2), siginterrupt(3)
#include <stdio.h> // for perror(3)
#include <errno.h> // for errno(3)

#include "us_helper.hh" // our user space helper

/*
 * This is an example of a calculator that you can break out of.
 * Notice that this example is written as a single thread and you can
 * still break out at any time from this calculator.
 * The idea is to setup a signal handler as interrupting syscalls
 * (in this case we use SIGUSR1). When that signal is received
 * Take heed to look at errno RIGHT AFTER the scanf fails since
 * any call to other APIs may change it's value and so lose the
 * original value you want to look at.
 *
 *		Mark Veltzer
 */

static void sig_handler(int sig) {
	//TRACE("start handler, sig is %d",sig);
	//TRACE("end");
}

int main(int argc, char **argv, char **envp) {
	// make sure we break out on receiving the SIGUSR1 signal...
	CHECK_NOT_M1(siginterrupt(SIGUSR1,1));
	CHECK_NOT_VAL(signal(SIGUSR1, sig_handler),SIG_ERR);
	int ret;
	printf("signal me with [kill -s SIGUSR1 %d]\n",getpid()); 
	bool broken=false;
	bool matchingError=false;
	while(true) {
		printf("enter number a: ");
		fflush(stdout);
		int a;
		ret=scanf("%d",&a);
		if(ret==EOF && errno==EINTR) {
			printf("\n\n\n\ninterrupted...shutting down...\n");
			broken=true;
			break;
		}
		if(ret!=1) {
			matchingError=true;
			break;
		}
		printf("enter number b: ");
		fflush(stdout);
		int b;
		ret=scanf("%d",&b);
		if(ret==EOF && errno==EINTR) {
			printf("\n\n\n\ninterrupted...shutting down...\n");
			broken=true;
			break;
		}
		if(ret!=1) {
			matchingError=true;
			break;
		}
		printf("a+b is %d\n",a+b);
		fflush(stdout);
	}
	if(broken) {
		printf("doing some cleanup code at end of program in case of break...\n");
	}
	if(matchingError) {
		printf("doing some cleanup code at end of program in case of EOF or parse error...\n");
	}
	return EXIT_SUCCESS;
}
