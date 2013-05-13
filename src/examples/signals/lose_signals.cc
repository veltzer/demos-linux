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
#include <signal.h>	// for SIGUSR1, SIGCHLD, kill(2)
#include <stdio.h>	// for printf(3), fprintf(3)
#include <unistd.h>	// for fork(2), pause(2), getppid(2)
#include <stdlib.h>	// for EXIT_SUCCESS, atoi(3)
#include <sys/types.h>	// for kill(2), getppid(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_ZERO(), CHECK_ASSERT(), register_handler_sigaction()

/*
 * This example shows that you can lose signals....
 */

static unsigned int got_signals;
static unsigned int max_sleep_time;
static bool child_alive=true;

static void handler(int sig, siginfo_t *si, void *unused) {
	if(sig==SIGUSR1) {
		got_signals++;
		//CHECK_NOT_M1(usleep(rand()%max_sleep_time));
		//CHECK_NOT_M1(sleep(1));
	}
	if(sig==SIGCHLD) {
		child_alive=false;
	}
}

void my_sleep(int seconds) {
	CHECK_ZERO(alarm(seconds));
	int ret=pause();
	CHECK_ASSERT(ret==-1 && errno==EINTR);
}

int main(int argc, char** argv, char** envp) {
	if(argc!=3) {
		fprintf(stderr, "%s: usage: %s [num_signals] [max_sleep_time]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// parameters
	unsigned int num_signals=atoi(argv[1]);
	max_sleep_time=atoi(argv[2]);
	// lets fork...
	if(CHECK_NOT_M1(fork())) {
		// parent
		// register signal handlers
		register_handler_sigaction(SIGUSR1, handler);
		register_handler_sigaction(SIGCHLD, handler);
		got_signals=0;
		while(child_alive) {
			int ret=pause();
			// this is what is guaranteed by a clean exit
			// of pause(2)
			CHECK_ASSERT(ret==-1 && errno==EINTR);
		}
		printf("parent: got %d signals...\n",num_signals);
		return EXIT_SUCCESS;
	} else {
		// child
		// no error return code for getppid(2)
		pid_t ppid=getppid();
		for(unsigned int i=0;i<num_signals;i++) {
			CHECK_NOT_M1(kill(SIGUSR1,ppid));
		}
		printf("child: send %d signals...\n",num_signals);
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
