#include <unistd.h> // for fork(2)
#include <stdio.h> // for fgets(3), perror(3)
#include <sys/types.h> // for waitid(2)
#include <sys/wait.h> // for waitid(2)
#include <stdlib.h> // for exit(3), atoi(3)
#include <string.h> // for strsignal(3)

#include "us_helper.hh"

/*
 * This example explains how parents should wait for their children
 * and analyze the result of the childs death or the like.
 * The example uses the most complex interface waitid(2). You could,
 * ofcourse, alter it to use one of the simpler apis:
 * wait, waitpid, wait3, wait4...
 *
 * Mark Veltzer
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
	if (child_pid == 0) {
		bool selected = false;
		int selection;
		while (!selected) {
			TRACE("this is the child");
			TRACE("Please select what do you want to do:");
			TRACE("1) exit(0)");
			TRACE("2) exit(1)");
			TRACE("3) do SIGSEGV");
			TRACE("- do 'kill -s SIGSTOP %d'", getpid());
			TRACE("- do 'kill -s SIGCONT %d'", getpid());
			char str[256];
			char *ret = fgets(str, 256, stdin);
			if (ret != str) {
				perror("could not get value");
				exit(1);
			}
			selection = atoi(str);
			if ((selection >= 1) && (selection <= 3)) {
				selected = true;
			}
		}
		switch (selection) {
		case 1:
			exit(0);
			break;

		case 2:
			exit(1);
			break;

		case 3:
			*((char *)0) = 0;
			break;
		}
	} else {
		TRACE("this is the parent");
		bool over = false;
		while (!over) {
			TRACE("waiting for the child...");
			siginfo_t info;
			int res = waitid(P_PID, child_pid, &info, WEXITED | WSTOPPED | WCONTINUED);
			if (res == -1) {
				perror("could not waitid(2)");
				exit(1);
			}
			print_code(info.si_code);
			print_status(info.si_status);
			if ((info.si_code == CLD_EXITED) || (info.si_code == CLD_KILLED)) {
				over = true;
			}
		}
	}
	return(0);
}
