#include <unistd.h> // for fork(2), getpid(2), sleep(3), pipe(2)
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
 * - catting 
 *
 * From the command line you can also see info about zombies using:
 * - ps(1) (done in this example).
 * - catting files from the /proc folder (done in two ways in this example).
 * - top(1)
 *
 *                              Mark Veltzer
 * EXTRA_LIBS=-lproc
 *
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

// print the state of a process in 3 different ways...
static inline void print_state(pid_t pid) {
	my_system("ps --no-headers -o comm,state %d",pid);
	my_system("cat /proc/%d/status | grep State",pid);
	proc_t myproc;
	get_proc_stats(pid,&myproc);
	printf("pid is %d, state is %c\n",pid, myproc.state);
}

int main(int argc, char **argv, char **envp) {
	pid_t child_pid;
	int pipefd_c2p[2];
	int pipefd_p2c[2];
	CHECK_ZERO(pipe(pipefd_c2p));
	CHECK_ZERO(pipe(pipefd_p2c));
	
	CHECK_NOT_M1(child_pid = fork());
	// the child
	if (child_pid == 0) {
		set_process_name("child");
		TRACE("this is the child");
		CHECK_ZERO(close(pipefd_c2p[0]));
		CHECK_ZERO(close(pipefd_p2c[1]));
		// tell the parent I am ready
		CHECK_1(write(pipefd_c2p[1],"r",1)); // r is for ready
		// lets wait for the parent to tell us to die...
		char c;
		CHECK_1(read(pipefd_p2c[0],&c,1));
		// now let the child die in order to become a zombie...
		return 0;
	// the parent
	} else {
		set_process_name("parent");
		TRACE("this is the parent");
		CHECK_ZERO(close(pipefd_c2p[1]));
		CHECK_ZERO(close(pipefd_p2c[0]));
		// wait for the child to become ready...
		char c;
		CHECK_1(read(pipefd_c2p[0],&c,1));
		// print the state of the child (in 3 different ways...)
		print_state(child_pid);
		// tell the child it is ok to die...
		CHECK_1(write(pipefd_p2c[1],"d",1)); // d is for die
		// sleep in order for the child to become a zombie...
		sleep(1);
		// print the state of the child (now he should be a zombie...).
		print_state(child_pid);
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
