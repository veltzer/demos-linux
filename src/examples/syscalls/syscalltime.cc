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
#include <stdio.h>	// for stderr, fprintf(3)
#include <unistd.h>	// for syscall(2), __NR_getpid, getpid(3)
#include <stdlib.h>	// for atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/syscall.h>// for syscall(2)
#include <sys/time.h>	// for gettimeofday(2), struct timeval
#include <us_helper.h>	// for run_priority()
#include <measure.h>	// for measure, measure_init(), measure_start(), measure_end(), measure_print()

/*
 * This demo times how long it takes to call a syscall.
 * It is a simple loop surrounded by gettimeofday.
 * We do call this code in high priority to make sure we do not get context switched
 * which will cause our measurements to be off.
 *
 * How long is it?
 * About 1/3 or even 1/5 mic on a fast computer.
 * About 1.2 mic on my old laptop.
 * About 0.5 mic on my new laptop.
 * Probably more on ARM processors although I am not sure.
 *
 * Notes:
 * - notice that syscall(__NR_getpid) is a little faster than gettimeofday. Obviously - it doesn't need
 * to get the time in the kernel.
 * - notice that getpid() is MUCH faster than syscall(__NR_getpid) because it caches the pid in user
 * space. It does not get optimized out (checked the disassembly).
 *
 * TODO:
 * - check that we indeed do not context switch out during the measurements.
 * this can be done using /proc/self/status and the nonvoluntary_ctxt_switches
 * parameter.
 * - allow to measure various system calls (not just gettimeofday) and learn
 * something from this.
 * - do a script that graphs the results of calling this code with various values of
 * 'count'. Show the convergence on the real value.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

unsigned int count;

void* func(void*) {
	measure m;
	// think! why count+1?
	measure_init(&m, "syscall", count+1);
	measure_start(&m);
	for(unsigned int i=0; i<count; i++) {
		// struct timeval t3;
		// gettimeofday(&t3, NULL);
		syscall(__NR_getpid);
		// getpid();
	}
	measure_end(&m);
	measure_print(&m);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [count]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	count=atoi(argv[1]);
	run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	return EXIT_SUCCESS;
}
