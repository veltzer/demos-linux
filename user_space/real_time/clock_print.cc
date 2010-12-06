#include <stdio.h> // for printf(3)
#include <time.h> // for clock_getres(2)

#include "us_helper.hh"

/*
 *	Print all clocks available on the system...
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lrt
 */

typedef struct _clock_data {
	const char name[256];
	clockid_t id;
} clock_data;

const int num_clocks = 4;
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
	}
};

int main(int argc, char **argv, char **envp) {
	for (int i = 0; i < num_clocks; i++) {
		clockid_t clk_id = clocks[i].id;
		struct timespec res;
		CHECK_ZERO(clock_getres(clk_id, &res));
		printf("clock resolution for clock %s (%d) is %ld (sec) %ld (nsec)\n", clocks[i].name, clocks[i].id, res.tv_sec, res.tv_nsec);
	}
	return(0);
}
