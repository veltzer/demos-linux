/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<stdio.h> // for printf(3)
#include<time.h> // for clock_getres(2)
#include<us_helper.h> // for CHECK_NOT_M1()
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Print all clocks available on the system for a user space app...
 * Notice that:
 * - different clocks show different times.
 * - different clocks have different resolutions.
 *
 * EXTRA_LIBS=-lrt
 */

typedef struct _clock_data {
	const char name[256];
	clockid_t id;
} clock_data;

const int num_clocks = 7;
// these clock ids are taken from /usr/include/bits/time.h...
clock_data clocks[num_clocks] = {
	{
		"CLOCK_REALTIME",
		CLOCK_REALTIME
	},
	{
		"CLOCK_MONOTONIC",
		CLOCK_MONOTONIC
	},
	{
		"CLOCK_PROCESS_CPUTIME_ID",
		CLOCK_PROCESS_CPUTIME_ID
	},
	{
		"CLOCK_THREAD_CPUTIME_ID",
		CLOCK_THREAD_CPUTIME_ID
	},
	{
		"CLOCK_MONOTONIC_RAW",
		CLOCK_MONOTONIC_RAW
	},
	{
		"CLOCK_REALTIME_COARSE",
		CLOCK_REALTIME_COARSE
	},
	{
		"CLOCK_MONOTONIC_COARSE",
		CLOCK_MONOTONIC_COARSE
	},
};

int main(int argc, char **argv, char **envp) {
	for(int i=0;i<num_clocks;i++) {
		clockid_t clk_id=clocks[i].id;
		struct timespec res,ts;
		CHECK_NOT_M1(clock_getres(clk_id, &res));
		CHECK_NOT_M1(clock_gettime(clk_id, &ts));
		printf("clock resolution for clock %s (%d) is %ld (sec) %ld (nsec), time %ld.%09ld\n", clocks[i].name, clocks[i].id, res.tv_sec, res.tv_nsec,ts.tv_sec,ts.tv_nsec);
	}
	return EXIT_SUCCESS;
}
