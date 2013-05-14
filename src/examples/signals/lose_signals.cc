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
#include <signal.h>	// for SIGUSR1, SIGCHLD, SIGRTMIN, kill(2)
#include <stdio.h>	// for printf(3), fprintf(3), stderr
#include <unistd.h>	// for fork(2), pause(2), getppid(2)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include <sys/types.h>	// for kill(2), getppid(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_ZERO(), CHECK_ASSERT(), register_handler_sigaction()
#include <us_signals.h>	// for signal_get_by_name()

/*
 * This example shows that you can lose signals....
 * The more you delay inside the signal handler, the more signals you lose.
 *
 * usleep inside the signal handler is problematic as it returns an error
 * if interrupted by a delivery of a signal. That is why I do not check
 * it's return value.
 *
 * sleep inside the signal handler is also problematic and also has
 * very low granularity (that is why it is not used).
 *
 * Notice that using RT signals in linux will make sure that you do not
 * lose signals.
 *
 * TODO:
 * - do some kind of hard delay function in us_helper.h which is like
 * the volatile loop below and call it "busy_delay" and use that
 * instead of usleep.
 * - try using nanosleep(2) inside the loop instead of usleep(2) or
 * sleep(2) or a busy delay.
 * - allow the user to select the signal to be sent from the command
 * line.
 */

static unsigned int got_signals;
static unsigned int max_sleep_time;
static bool child_alive=true;
static int the_signal=-1;

static void handler(int sig, siginfo_t *si, void *context) {
	// printf("started hander\n");
	if(sig==the_signal) {
		got_signals++;
		/*
		 * volatile unsigned long long sum=0;
		 * for(int i=0;i<100000000;i++) {
		 *      for(int j=0;j<10;j++) {
		 *              sum+=j*j;
		 *      }
		 * }
		 */
		// if you pass 0 to % you will get a floating
		// point exception...
		if(max_sleep_time>0) {
			// I do not check the return code on purpose
			// see the noted above!
			usleep(rand()%max_sleep_time);
		}
	}
	if(sig==SIGCHLD) {
		child_alive=false;
	}
	// printf("ended hander\n");
}

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [num_signals] [max_sleep_time] [signal_name]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 1000 1000 SIGUSR1\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 1000 0 SIGUSR1\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 1000 1000 SIGRTMIN\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// parameters
	unsigned int num_signals=atoi(argv[1]);
	max_sleep_time=atoi(argv[2]);
	the_signal=signal_get_by_name(argv[3]);
	// lets fork...
	if(CHECK_NOT_M1(fork())) {
		// parent
		// register signal handlers
		register_handler_sigaction(the_signal, handler);
		register_handler_sigaction(SIGCHLD, handler);
		got_signals=0;
		// a non busy busy wait loop
		while(child_alive) {
			int ret=pause();
			// this is what is guaranteed by a clean exit
			// of pause(2)
			CHECK_ASSERT(ret==-1 && errno==EINTR);
		}
		printf("parent: got %d signals...\n", got_signals);
	} else {
		// child
		// no error return code for getppid(2)
		pid_t ppid=getppid();
		for(unsigned int i=0; i<num_signals; i++) {
			CHECK_NOT_M1(kill(ppid, the_signal));
		}
		printf("child: send %d signals...\n", num_signals);
	}
	return EXIT_SUCCESS;
}
