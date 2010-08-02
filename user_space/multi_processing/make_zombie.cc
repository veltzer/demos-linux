#include <unistd.h> // for fork(2), getpid(2), sleep(3)
#include <stdio.h> // for fgets(3), perror(3)
#include <sys/types.h> // for waitid(2), getpid(2)
#include <sys/wait.h> // for waitid(2)
#include <stdlib.h> // for exit(3)
#include <string.h> // for strsignal(3)

#include "us_helper.hh"

/*
 * This example demostrates how processes become zombies in Linux...
 *
 * How does a process become a zombie in Linux ?
 * - The parent creates the process.
 * - The process ends.
 * - The parent does not collect the processes return code using one of
 *   the wait(2) family of functions.
 *
 * Note that if the parent dies then init(1) will actually serve as the
 * new parent for the child process (sort of adoption) and will clear the
 * child. So a zombie is always a process with a live parent (unless there
 * is a bug in init(1), ofcourse...).
 *
 * We show the status of the child in this application in two ways:
 * - using the ps command.
 * - using the libproc library.
 *
 * From the command line you can see the same info using:
 * - catting files from the /proc folder.
 * - ps(1)
 * - top(1)
 *
 *                              Mark Veltzer
 * EXTRA_LIBS=-lproc
 */
void print_status(int status) {
	if (WIFEXITED(status)) {
		TRACE("child exited normally with status %d", WEXITSTATUS(status));
	}
	if (WIFSTOPPED(status)) {
		TRACE("child was stopped with signal %s", strsignal(WSTOPSIG(status)));
	}
	if (WIFSIGNALED(status)) {
		TRACE("child was signaled with signal %s", strsignal(WTERMSIG(status)));
	}
}


void print_code(int code) {
	switch (code) {
	case CLD_EXITED:
		TRACE("child exited of it's own accord");
		break;

	case CLD_KILLED:
		TRACE("child was killed");
		break;

	case CLD_STOPPED:
		TRACE("child was stopped");
		break;

	case CLD_CONTINUED:
		TRACE("child was continued");
		break;
	}
}

int main(int argc, char **argv, char **envp) {
	TRACE("this is the parent");
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("could not fork");
		exit(1);
	}
	// the child
	if (child_pid == 0) {
		TRACE("this is the child, pid is %d",getpid());
		// lets simulate some work...
		sleep(6);
		// now let the child die in order to become a zombie...
		return 0;
	} else {
		TRACE("this is the parent, pid is %d",getpid());
		// lets make the parent do something and forget about the child
		for(int i=0;i<10;i++) {
			sleep(2);
			my_system("ps -o cmd,state %d",child_pid);
			print_stats(child_pid);
		}
		// now lets take the return code from the child...
		siginfo_t info;
		int res = waitid(P_PID, child_pid, &info, WEXITED | WSTOPPED | WCONTINUED);
		if (res == -1) {
			perror("could not waitid(2)");
			exit(1);
		}
		print_code(info.si_code);
		print_status(info.si_status);
	}
	return(0);
}
