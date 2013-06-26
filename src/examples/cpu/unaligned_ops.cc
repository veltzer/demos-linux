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
#include <stdlib.h>	// for EXIT_SUCCESS, malloc(3), free(3)
#include <measure.h>	// for measure, measure_start(), measure_end(), measure_print()
#include <us_helper.h>	// for run_priority()

/*
 * This example tries to abuse the cpu by doing lots of integral
 * operations on addresses which are not 4 byte aligned and showing
 * the difference in performace between these and properly aligned
 * operations.
 *
 * Results:
 * It seems that on Intel data alignment is not an issue.
 *
 * References:
 * - http://lemire.me/blog/archives/2012/05/31/data-alignment-for-speed-myth-or-reality/
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

static int diff;
static char* buf;
const unsigned int num_of_ints=1000;

void* func(void*) {
	measure m;
	measure_start(&m, "work");
	int* pi=(int*)(buf+diff);
	for(int j=0; j<1000000; j++) {
		for(int i=0; i<1000; i++) {
			pi[i]+=i*i;
		}
	}
	measure_end(&m, "work");
	printf("diff is %d\n", diff);
	measure_print(&m, "work", 1);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	unsigned int size_to_alloc=num_of_ints*sizeof(int)+500;
	buf=(char*)malloc(size_to_alloc);
	for(int i=0; i<2020; i++) {
		buf[i]=rand()%256;
	}
	diff=0;
	run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	diff=1;
	run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	diff=2;
	run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	diff=3;
	run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	free(buf);
	return EXIT_SUCCESS;
}
