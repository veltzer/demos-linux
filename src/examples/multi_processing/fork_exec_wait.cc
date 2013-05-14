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

#include <firstinclude.h>
#include <unistd.h>	// for fork(2), execlp(2)
#include <stdlib.h>	// for EXIT_SUCCCESS
#include <sys/types.h>	// for wait(2)
#include <sys/wait.h>	// for wait(2)
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <multi_processing.h>	// for print_status()
#include <stdio.h>	// for printf(3)

int main(int argc, char** argv, char** envp) {
	pid_t pid=CHECK_NOT_M1(fork());
	if(pid) {
		// parent
		int status;
		pid_t child_that_died=CHECK_NOT_M1(wait(&status));
		printf("child died with pid=%d\n", child_that_died);
		print_status(status);
		return EXIT_SUCCESS;
	} else {
		// child
		CHECK_NOT_M1(execlp("/bin/ls", "/bin/ls", "-l", NULL));
		return EXIT_SUCCESS;
	}
}
