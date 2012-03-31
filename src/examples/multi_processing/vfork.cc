#include<unistd.h> // for fork(2)
#include<stdio.h> // for fgets(3), perror(3)
#include<sys/types.h> // for waitid(2)
#include<sys/wait.h> // for waitid(2)
#include<stdlib.h> // for exit(3), atoi(3)
#include<string.h> // for strsignal(3)
#include<sys/types.h> // for vfork(2)
#include<unistd.h> // for vfork(2)

#include"us_helper.hh"

/*
 * An example of using vfork(2)
 *
 * Notes:
 * - if you let the child return from main it will cause a problem.
 * - tracing seems to work fine and sleep(3) also.
 * - writing to the parent data does not seem to be harmful too.
 * - result: the return from the function in which vfork was called is the one
 * causing the segfault.
 * - the segfault seems to be in the parent not in the child. Need to check this
 * for sure.
 * - the results of using vfork incorrectly (not for exec) are indeed undefined as the manual page says.
 *
 *		Mark Veltzer
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

int global_data=42;

int main(int argc, char **argv, char **envp) {
	TRACE("this is the parent");
	pid_t child_pid;
	CHECK_NOT_M1(child_pid=vfork());
	if (child_pid == 0) {
		TRACE("this is the child");
		global_data++;
		//while(true) {
		//}
		sleep(10);
	} else {
		TRACE("this is the parent");
		bool over = false;
		while(!over) {
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
	return EXIT_SUCCESS;
}
