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
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for sleep(3)
#include <papi.h>	// for PAPI_* functions
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * Demo for using the PAPI performance counters library.
 *
 * References:
 * http://icl.cs.utk.edu/papi/index.html
 *
 * EXTRA_LINK_FLAGS=-lpapi
 */

void print_counters(long long* arr, int num) {
	for(int i=0; i<num; i++) {
		printf("%d: %llu\n", i, arr[i]);
	}
}

int main(int argc, char** argv, char** envp) {
	printf("starting up...\n");
	int counters=PAPI_num_counters();
	printf("PAPI_num_counters() returned %d\n", counters);
	int components=PAPI_num_components();
	printf("PAPI_num_components() returned %d\n", components);

	// array for counters
	long long* arr=new long long[counters];
	float rtime, ptime, ipc, mflops;
	long long ins, flpops;

	int ret=PAPI_ipc(&rtime, &ptime, &ins, &ipc);
	printf("PAPI_ipc() returned:\n");
	printf("\trtime=%f\n", rtime);
	printf("\tptime=%f\n", ptime);
	printf("\tins=%lld\n", ins);
	printf("\tipc=%f\n", ipc);
	printf("\tret=%d\n", ret);

	ret=PAPI_flops(&rtime, &ptime, &flpops, &mflops);
	printf("PAPI_flops() returned:\n");
	printf("\trtime=%f\n", rtime);
	printf("\tptime=%f\n", ptime);
	printf("\tflpops=%lld\n", flpops);
	printf("\tmflops=%f\n", mflops);
	printf("\tret=%d\n", ret);

	ret=PAPI_read_counters(arr, counters);
	printf("ret is %d\n", ret);
	print_counters(arr, counters);
	sleep(1);
	ret=PAPI_read_counters(arr, counters);
	printf("ret is %d\n", ret);
	print_counters(arr, counters);
	return EXIT_SUCCESS;
}
