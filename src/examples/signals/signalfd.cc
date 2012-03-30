#include<sys/signalfd.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

/*
 * This demo of signalfd was stolen shamelessly from the signalfd(2) manpage.
 *
 * 			Mark Veltzer
 */

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char** argv, char** envp) {
	sigset_t mask;
	int sfd;
	struct signalfd_siginfo fdsi;
	ssize_t s;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);

	/* Block signals so that they aren't handled
		according to their default dispositions */
	if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
		handle_error("sigprocmask");
	sfd = signalfd(-1, &mask, 0);
	if (sfd == -1)
		handle_error("signalfd");

	while(true) {
		s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
		if (s != sizeof(struct signalfd_siginfo))
			handle_error("read");

		if (fdsi.ssi_signo == SIGINT) {
			printf("Got SIGINT\n");
		} else if (fdsi.ssi_signo == SIGQUIT) {
			printf("Got SIGQUIT\n");
			exit(EXIT_SUCCESS);
		} else {
			printf("Read unexpected signal\n");
		}
	}
	return EXIT_SUCCESS;
}
