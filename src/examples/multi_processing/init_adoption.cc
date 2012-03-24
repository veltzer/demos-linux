#include <unistd.h> // for fork(2), getpid(2), sleep(3), getppid(2)
#include <stdio.h> // for fgets(3), perror(3)
#include <sys/types.h> // for waitid(2), getpid(2), getppid(2)
#include <sys/wait.h> // for waitid(2)
#include <stdlib.h> // for exit(3)
#include <string.h> // for strsignal(3)
#include <signal.h> // for kill(2)

#include "us_helper.hh"

/*
 * This example demostrates what happens when a processes father dies...
 *
 * - Notice that the getppid() function returns different values because of the
 * adoption by the init(1) process.
 * - This example forks twice to show that there is no "grandparent adoption" and
 * that grandparents are not interested in their grandchildren.
 * - We need to wait a little to make sure that the parent dies - there is no synchroneous
 * way to get this info as far as I know...
 *
 * TODO:
 * - investigate how it is exactly that init does the things he does (is it the kernel as I
 * suspect that latches processes under the init process ?!? - if so, find the place
 * in the kernel that does this...).
 *
 *	Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("could not fork");
		exit(1);
	}
	if (child_pid == 0) {
		pid_t gchild_pid = fork();
		if (gchild_pid == -1) {
			perror("could not fork");
			exit(1);
		}
		if (gchild_pid == 0) {
			TRACE("this is the gchild, pid is %d",getpid());
			// lets show the parent pid...
			TRACE("my parent pid is %d",getppid());
			// now lets signal our parent that its ok to die...
			CHECK_NOT_M1(kill(getppid(),SIGUSR1));
			// lets wait a while to make sure the parent really dies..
			sleep(1);
			// now lets print our parent again...
			TRACE("my parent pid is %d",getppid());
			return 0;
		} else {
			TRACE("this is the parent, pid is %d",getpid());
			// lets wait for a signal that it's ok to die...
			pause();
			// lets die
			return 0;
		}
	} else {
		TRACE("this is the gparent, pid is %d",getpid());
		// lets wait for a signal that it's ok to die...
		pause();
		// lets die
		return 0;
	}
	return(0);
}
