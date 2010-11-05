#include<sys/eventfd.h> // for eventfd(2)
#include<unistd.h> // for fork(2), close(2), sleep(3), write(2), read(2)
#include<stdlib.h> // for exit(3)
#include<stdio.h> // for fprintf(3), strtoull(3)
#include<signal.h> // for siginterrupt(2), signal(2)

#include"us_helper.hh"

/*
 * This program demos parent child communication via an event fd
 * This program was shamelssly stolen and modified from the eventfd
 * manpage
 *
 * Notes:
 * - the child may make several calls to write and the parent will only
 *   get one wakeup for these calls. This is because eventfd is made for
 *   signaling and two signals are just as good as one. The parent can
 *   notice that it has been signaled more than once since the sum of the
 *   childs passed values is passed to it.
 * - eventfd can be multiplexed using select, poll or epoll.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */

volatile bool cont=true;

void handler(int signum) {
	fprintf(stderr,"got SIGCHLD\n");
	cont=false;
}

int main(int argc, char **argv, char **envp) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [series of numbers to send]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int efd=eventfd(0, 0);
	scie(efd,"eventfd");

	pid_t pid=fork();
	scie(pid,"fork");

	if(pid==0) {
		// child branch
		for (int j = 1; j < argc; j++) {
			uint64_t u = strtoull(argv[j], NULL, 0);
			printf("Child writing %llu (0x%llx) to efd\n",u,u);
			ssize_t s = write(efd, &u, sizeof(uint64_t));
			scassert(s == sizeof(uint64_t),"write size");
			//sleep(1);
		}
		printf("Child completed write loop\n");
		scie(close(efd),"close");
		printf("Child exiting\n");
		return(0);
	} else {
		// parent branch
		// install a signal handler for when the child dies so that we could know
		// that we need to stop listening for messages from it
		sighandler_t old=signal(SIGCHLD,handler);
		scassert(old!=SIG_ERR,"signal");
		// this is neccessary in order to 'break' out of the read(2) system
		// call when SIGCHLD comes along...
		scie(siginterrupt(SIGCHLD,1),"siginterrupt");
		printf("Parent about to read\n");
		while(cont) {
			uint64_t u;
			ssize_t s = read(efd, &u, sizeof(uint64_t));
			if(cont) {
				scassert(s == sizeof(uint64_t),"read size");
				printf("Parent read %llu (0x%llx) from efd\n",u,u);
			}
		}
		scie(close(efd),"close");
		printf("Parent exiting\n");
		return(0);
	}
}
