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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <sched.h>	// for sched_setscheduler(2), sched_param
#include <sys/types.h>	// for pid_t

/*
 * make a real time process return and be a regular process.
 *
 * This is similar to chrt(1) from the util-linux package which is
 * a command line utility. (although that chrt has lots of features...)
 */

int main(int argc, char** argv, char** envp) {
	if(argc<2) {
		fprintf(stderr, "%s: Usage: %s [pid]...\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	for(int i=1; i<argc; i++) {
		struct sched_param sp={sched_priority : 0};
		pid_t pid=atoi(argv[i]);
		CHECK_NOT_M1(sched_setscheduler(pid, SCHED_OTHER, &sp));
	}
	return EXIT_SUCCESS;
}
