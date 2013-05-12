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
#include <sys/timerfd.h>// for timerfd_create(2), timerfd_settime(2), timerfd_gettime(2)
#include <unistd.h>	// for read(2)
#include <time.h>	// for clock_gettime(2)
#include <stdlib.h>	// for exit(3), EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>	// for printf(3), fprintf(3)
#include <stdint.h>	// for uint64_t
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This example was stolen shamelessly from the timerfd_create(2) manpage,
 * and adapted to my coding style.
 *
 * EXTRA_LINK_FLAGS=-lrt
 */

static void print_elapsed_time(void) {
	static struct timespec start;
	struct timespec curr;
	static int first_call=1;
	int secs, nsecs;
	if (first_call) {
		first_call=0;
		CHECK_NOT_M1(clock_gettime(CLOCK_MONOTONIC, &start));
	}
	CHECK_NOT_M1(clock_gettime(CLOCK_MONOTONIC, &curr));
	secs=curr.tv_sec - start.tv_sec;
	nsecs=curr.tv_nsec - start.tv_nsec;
	if (nsecs < 0) {
		secs--;
		nsecs+=1000000000;
	}
	printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}

int main(int argc, char** argv, char** envp) {
	if (argc!=4) {
		fprintf(stderr, "%s: usage: %s [init-secs] [interval-secs] [max-exp]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 2 5 6\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}
	struct timespec now;
	CHECK_NOT_M1(clock_gettime(CLOCK_REALTIME, &now));
	/*
	 * Create a CLOCK_REALTIME absolute timer with initial
	 * expiration and interval as specified in command line
	 */
	struct itimerspec new_value;
	uint64_t max_exp;
	new_value.it_value.tv_sec=now.tv_sec + atoi(argv[1]);
	new_value.it_value.tv_nsec=now.tv_nsec;
	new_value.it_interval.tv_sec=atoi(argv[2]);
	max_exp=atoi(argv[3]);
	new_value.it_interval.tv_nsec=0;

	int fd=CHECK_NOT_M1(timerfd_create(CLOCK_REALTIME, 0));
	CHECK_NOT_M1(timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL));
	print_elapsed_time();
	printf("timer started\n");
	uint64_t tot_exp;
	for(tot_exp=0; tot_exp<max_exp; ) {
		uint64_t exp;
		CHECK_INT(read(fd, &exp, sizeof(uint64_t)), sizeof(uint64_t));
		tot_exp+=exp;
		print_elapsed_time();
		printf("read: %llu; total=%llu\n", (unsigned long long) exp, (unsigned long long) tot_exp);
	}
	return EXIT_SUCCESS;
}
