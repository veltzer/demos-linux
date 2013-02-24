/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for atoi(3)
#include <stdio.h>	// for printf(3)
#include <sys/types.h>	// for WIF(3)
#include <sys/wait.h>	// for WIF(3)

/*
 * This executable receives the status code (exit code) of some process and prints
 * an analysis of what the code means. Useful in case where you are not sure what
 * the status code means or as a demo of how to analyze such a code.
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		printf("usage: %s [status code]\n", argv[0]);
		return -1;
	}
	int res=atoi(argv[1]);
	printf("analyzing code %d\n", res);
	if (WIFSIGNALED(res)) {
		printf("ahm... Child was killed by os with signal %d\n", WTERMSIG(res));
	}
	if (WIFEXITED(res)) {
		printf("Child was NOT killed by OS.\n");
		int return_code=WEXITSTATUS(res);
		if (return_code) {
			printf("Child exited successfully but reported error %d\n", return_code);
		} else {
			printf("Child was a success\n");
		}
	}
	return EXIT_SUCCESS;
}
