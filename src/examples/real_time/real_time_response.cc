/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for fprintf(3), stderr
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <time.h>	// for clock_gettime(2), clock_nanosleep(2), CLOCK_REALTIME, CLOCK_MONOTONIC
#include <pthread_utils.h>	// for pthread_stack_prefault()
#include <timespec_utils.h>	// for timespec_add_nanos(), timespec_sub(), timespec_nanos()
#include <sched.h>	// for sched_setscheduler(2), struct sched_param
#include <sys/mman.h>	// for mlockall(2)
#include <err_utils.h>	// for CHECK_NOT_M1()

/*
 * This example explores the responsiveness of the OS.
 *
 * TODO:
 * - add the ability to get the clock via the command line too.
 *
 * EXTRA_LINK_FLAGS=-lpthread -lrt
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [internal_ns] [requirement_ns] [priority]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 50000 10000 49\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// get the parameters
	const int interval=atoi(argv[1]);
	const unsigned long long requirement=atoi(argv[2]);
	const int priority=atoi(argv[3]);

	// prep code 
	/* Declare ourself as a real time task */
	struct sched_param param;
	param.sched_priority=priority;
	CHECK_NOT_M1(sched_setscheduler(0, SCHED_FIFO, &param));
	/* Lock memory */
	CHECK_NOT_M1(mlockall(MCL_CURRENT|MCL_FUTURE));
	/* Pre-fault our stack - this is useless because of mlock(2) */
	pthread_stack_prefault();
	int clock=CLOCK_MONOTONIC;
	//int clock=CLOCK_REALTIME;

	// start loop
	/* get the current time */
	struct timespec t;
	CHECK_NOT_M1(clock_gettime(clock, &t));
	/* start after one second */
	timespec_add_nanos(&t, interval);
	int success=0;
	while(true) {
		/* wait untill next shot */
		CHECK_NOT_M1(clock_nanosleep(clock, TIMER_ABSTIME, &t, NULL));
		struct timespec now;
		CHECK_NOT_M1(clock_gettime(clock, &now));
		unsigned long long diff_nanos=timespec_diff_nano(&now, &t);
		if(diff_nanos>requirement) {
			fprintf(stderr,"success count is %d\n", success);
			fprintf(stderr,"diff nanos is %llu\n", diff_nanos);
			return EXIT_FAILURE;
		}
		success++;
		//CHECK_ASSERT(diff_nanos<10000);
		/* do the stuff
		 * ...
		 * calculate next shot */
		timespec_add_nanos(&t, interval);
	}
	return EXIT_SUCCESS;
}
