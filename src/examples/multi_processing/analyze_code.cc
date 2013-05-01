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
#include <stdlib.h>	// for atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>	// for printf(3), fprintf(3)
#include <sys/types.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)
#include <sys/wait.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)

/*
 * This executable receives the status code (exit code) of some process and prints
 * an analysis of what the code means. Useful in case where you are not sure what
 * the status code means or as a demo of how to analyze such a code.
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [status code]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: 0 - the process ended with success\n", argv[0]);
		fprintf(stderr, "%s: n*256 - the process ended with error n\n", argv[0]);
		fprintf(stderr, "%s: n<256 - the process was killed by OS signal n\n", argv[0]);
		return EXIT_FAILURE;
	}
	int res=atoi(argv[1]);
	printf("analyzing code [%d]\n", res);
	if (WIFSIGNALED(res)) {
		printf("Child was killed by os with signal [%d]\n", WTERMSIG(res));
	}
	if (WIFEXITED(res)) {
		printf("Child was NOT killed by OS\n");
		int return_code=WEXITSTATUS(res);
		if (return_code) {
			printf("Child exited but reported error [%d]\n", return_code);
		} else {
			printf("Child was a success\n");
		}
	}
	return EXIT_SUCCESS;
}
