/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

/*
 * This is a solution to the watchdog exercise
 *
 * This solution is better in that it is based on signal handling
 * and does not block the parent. Instead the parent is doing
 * a non-busy ever loop using pause(2).
 */

#include <firstinclude.h>
#include <unistd.h>	// for fork(2), execv(2), pause(2)
#include <stdlib.h>	// for EXIT_SUCCCESS
#include <signal.h>	// for SIGCHLD
#include <sys/types.h>	// for waitpid(2), WIFSIGNALED(3)
#include <sys/wait.h>	// for waitpid(2), WIFSIGNALED(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), TRACE(), register_handler_sigaction(), CHECK_ASSERT()
#include <multi_processing.h>	// for print_status()

const char* process_to_exec="src/exercises/watchdog/process_to_monitor.elf";
const char* const args[]={
	process_to_exec
};

static bool fork_again;
static pid_t child_pid;

static void handler(int sig, siginfo_t *si, void *unused) {
	int status;
	// WNOHANG is here to make sure that we do not hang. Since this is a SIGCHLD
	// handler then we can be certain that the first call to waitpid(2) will not
	// hang. If we were monitoring more than one child, and because we are afraid
	// of signal loss, we would do a 'pumping loop' here...
	pid_t child_that_died=CHECK_NOT_M1(waitpid(child_pid, &status, WNOHANG));
	CHECK_ASSERT(child_that_died==child_pid);
	child_pid=-1;
	TRACE("child died with pid=%d", child_that_died);
	print_status(status);
	if(WIFSIGNALED(status)) {
		fork_again=true;
	}
}

static void fork_a_child() {
	TRACE("forking a child");
	child_pid=CHECK_NOT_M1(fork());
	if(child_pid) {
		// parent
		fork_again=false;
		return;
	}
	// child
	CHECK_NOT_M1(execv(process_to_exec, (char* const*)args));
}

int main(int argc, char** argv, char** envp) {
	register_handler_sigaction(SIGCHLD, handler);
	fork_a_child();
	TRACE("parent starts monitoring");
	while(true) {
		int ret=pause();
		// this is what is guaranteed by a clean exit
		// of pause(2)
		CHECK_ASSERT(ret==-1 && errno==EINTR);
		if(fork_again)
			fork_a_child();
	}
	return EXIT_SUCCESS;
}
