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
 */

#include <firstinclude.h>
#include <unistd.h>	// for fork(2), execv(2)
#include <stdlib.h>	// for EXIT_SUCCCESS
#include <sys/types.h>	// for wait(2), WIFSIGNALED(3)
#include <sys/wait.h>	// for wait(2), WIFSIGNALED(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), TRACE()
#include <multi_processing.h>	// for print_status()

const char* process_to_exec="src/exercises/watchdog/process_to_monitor.elf";
const char* const args[]={
	process_to_exec
};

void fork_a_child() {
	TRACE("forking a child");
	pid_t pid=CHECK_NOT_M1(fork());
	if(pid) {
		// parent
		return;
	}
	// child
	CHECK_NOT_M1(execv(process_to_exec, (char* const*)args));
}

int main(int argc, char** argv, char** envp) {
	fork_a_child();
	TRACE("parent starts monitoring");
	while(true) {
		int status;
		pid_t child_that_died=CHECK_NOT_M1(wait(&status));
		TRACE("child died with pid=%d", child_that_died);
		print_status(status);
		if(WIFSIGNALED(status)) {
			fork_a_child();
		}
	}
	return EXIT_SUCCESS;
}
