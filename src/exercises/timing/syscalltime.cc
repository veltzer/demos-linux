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
#include <stdio.h>	// for printf(3), fprintf(3)
#include <stdlib.h>	// for atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/time.h>	// for gettimeofday(2)
#include <us_helper.h>	// for micro_diff(), run_high_priority()

/*
 * A solution to the timing exercise (timing gettimeofday(2) using gettimeofday(2)).
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

unsigned int count;

void* func(void*) {
	struct timeval t1, t2;
	printf("doing %d syscalls\n", count);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0; i<count; i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1, &t2)/(double)(count+1));
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [count]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	count=atoi(argv[1]);
	run_high_priority(func, NULL, STANDARD_HIGH_PRIORITY);
	return EXIT_SUCCESS;
}
