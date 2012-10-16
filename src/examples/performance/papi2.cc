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

#include<firstinclude.h>
#include<papi.h> // for PAPI_* functions
#include<unistd.h> // for sleep(3)
#include<stdio.h> // for printf(3), perror(3)
#include<stdlib.h> // for exit(3), atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include<errno.h> // for perror(3)

/*
* Demo for using the PAPI performance counters library.
*
* References:
* http://icl.cs.utk.edu/papi/index.html
*
* EXTRA_LIBS=-lpapi
*/

void handle_error(int val) {
	perror("had an error");
	exit(EXIT_FAILURE);
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
	int Events[NUM_EVENTS]={PAPI_TOT_INS, PAPI_TOT_CYC};
	long_long values[NUM_EVENTS];
	/* Start counting events */
	if (PAPI_start_counters(Events, NUM_EVENTS)!=PAPI_OK)
		handle_error(1);
	/* Do some computation here */
	//sleep(atoi(argv[1]));
	dummy_work(atoi(argv[1]));
	/* Stop counting events */
	if (PAPI_stop_counters(values, NUM_EVENTS)!=PAPI_OK)
		handle_error(1);
	printf("value 0 is %lld\n",values[0]);
	printf("value 1 is %lld\n",values[1]);
	return EXIT_SUCCESS;
}
