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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <sched_utils.h>// for sched_run_priority()
#include <measure.h>	// for measure, measure_init(), measure_start(), measure_end(), measure_print()

/*
 * This example explores the performance of compiler barriers...
 *
 * References:
 * https://blogs.oracle.com/d/entry/compiler_memory_barriers
 *
 * this is for the source interleaving below...
 * EXTRA_COMPILE_FLAGS=-g3
 * we need threads for the high priority...
 * EXTRA_LINK_FLAGS=-lpthread
 *
 * TODO:
 * - this example does not work since the compiler still optimizes away the barrier() function.
 */

void barrier() __attribute__((noinline));
void barrier() {
}

void* func(void*) {
	const int loop=1000000000;
	measure m;
	measure_init(&m, "function barrier", loop);
	measure_start(&m);
	for(int j=0; j<loop; j++) {
		barrier();
	}
	measure_end(&m);
	measure_print(&m);
	measure_init(&m, "compiler barrier", loop);
	measure_start(&m);
	for(int j=0; j<loop; j++) {
		asm volatile ("":::"memory");
	}
	measure_end(&m);
	measure_print(&m);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	sched_run_priority(func, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	return EXIT_SUCCESS;
}
