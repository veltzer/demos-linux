#include <stdio.h> // for printf(3)
#include <time.h> // for clock_getres(2)

#include "us_helper.hh"

/*
 *	Print all clocks available on the system for a user space app...
 *	Notice that:
 *	- different clocks show different times.
 *	- different clocks have different resolutions.
 *
 *		Mark Veltzer
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
	for (int i = 0; i < num_clocks; i++) {
		clockid_t clk_id = clocks[i].id;
		struct timespec res,ts;
		CHECK_NOT_M1(clock_getres(clk_id, &res));
		CHECK_NOT_M1(clock_gettime(clk_id, &ts));
		printf("clock resolution for clock %s (%d) is %ld (sec) %ld (nsec), time %ld.%09ld\n", clocks[i].name, clocks[i].id, res.tv_sec, res.tv_nsec,ts.tv_sec,ts.tv_nsec);
	}
	return EXIT_SUCCESS;
}
