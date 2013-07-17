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
#include <stdio.h>	// for printf(3)
#include <time.h>	// for clock_getres(2), clock_gettime(2), struct timespec
#include <err_utils.h>	// for CHECK_NOT_M1()
#include <us_helper.h>	// for ARRAY_SIZEOF()
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * Print all clocks available on the system for a user space app...
 * Notice that:
 * - different clocks show different times.
 * - different clocks have different resolutions.
 *
 * EXTRA_LINK_FLAGS=-lrt
 */

typedef struct _clock_data {
	const char name[256];
	clockid_t id;
} clock_data;

// these clock ids are taken from /usr/include/bits/time.h...
clock_data clocks[]={
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

int main(int argc, char** argv, char** envp) {
	for(unsigned int i=0; i<ARRAY_SIZEOF(clocks); i++) {
		clockid_t clk_id=clocks[i].id;
		struct timespec res, ts;
		CHECK_NOT_M1(clock_getres(clk_id, &res));
		CHECK_NOT_M1(clock_gettime(clk_id, &ts));
		printf("clock resolution for clock %s (%d) is %ld (sec) %ld (nsec), time %ld.%09ld\n", clocks[i].name, clocks[i].id, res.tv_sec, res.tv_nsec, ts.tv_sec, ts.tv_nsec);
	}
	return EXIT_SUCCESS;
}
