#include<papi.h> // for PAPI_* functions
#include<unistd.h> // for sleep(3)
#include<stdio.h> // for printf(3), perror(3)
#include<stdlib.h> // for exit(3), atoi(3)
#include<errno.h> // for perror(3)

/*
 * Demo for using the PAPI performance counters library.
 * 
 * References:
 * http://icl.cs.utk.edu/papi/index.html
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lpapi
 */

void handle_error(int val) {
	perror("had an error");
	exit(1);
}

long long dummy_work(int limit) {
	int count=0;
	for(int j=0;j<limit;j++) {
		for(int i=0;i<limit;i++) {
			count+=dummy_work(limit-1);
			//count+=i;
		}
	}
	return count;
}

int main(int argc,char** argv,char** envp) {
	const unsigned int NUM_EVENTS=2;
	int Events[NUM_EVENTS] = {PAPI_TOT_INS, PAPI_TOT_CYC};
	long_long values[NUM_EVENTS];
	/* Start counting events */
	if (PAPI_start_counters(Events, NUM_EVENTS) != PAPI_OK)
		handle_error(1);
	/* Do some computation here */
	//sleep(atoi(argv[1]));
	dummy_work(atoi(argv[1]));
	/* Stop counting events */
	if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK)
		handle_error(1);
	printf("value 0 is %lld\n",values[0]);
	printf("value 1 is %lld\n",values[1]);
	return EXIT_SUCCESS;
}
