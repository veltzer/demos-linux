/*
 * This file is part of the linuxapi project.
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
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/time.h>	// for gettimeofday(2)
#include <us_helper.h>	// for TRACE(), micro_diff, CHECK_ZERO

/*
 * This example compares the adding of integers and the adding of atomics
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [attempts]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	int attempts=atoi(argv[1]);
	int counter;
	volatile int vcounter;
	struct timeval t1, t2;
	TRACE("attempts is %d", attempts);

	counter=0;
	gettimeofday(&t1, NULL);
	for(int i=0; i<attempts; i++) {
		__sync_add_and_fetch(&counter, 1);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of atomic adds: %lf\n", micro_diff(&t1, &t2)/(double)attempts);

	counter=0;
	gettimeofday(&t1, NULL);
	for(int i=0; i<attempts; i++) {
		counter++;
		// this is to make the compiler actually do the loop
		asm volatile ("" ::: "memory");
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of regular adds (big barrier): %lf\n", micro_diff(&t1, &t2)/(double)attempts);

	counter=0;
	gettimeofday(&t1, NULL);
	for(int i=0; i<attempts; i++) {
		counter++;
		// this is to make the compiler actually do the loop
		asm volatile ("" : "=g" (counter) ::);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of regular adds (best barrier): %lf\n", micro_diff(&t1, &t2)/(double)attempts);

	counter=0;
	gettimeofday(&t1, NULL);
	for(int i=0; i<attempts; i++) {
		counter++;
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of regular adds (no barrier-the loop probably goes away here): %lf\n", micro_diff(&t1, &t2)/(double)attempts);

	vcounter=0;
	gettimeofday(&t1, NULL);
	for(int i=0; i<attempts; i++) {
		vcounter++;
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of volatile adds (volatile is close to a barrier): %lf\n", micro_diff(&t1, &t2)/(double)attempts);
	return EXIT_SUCCESS;
}
