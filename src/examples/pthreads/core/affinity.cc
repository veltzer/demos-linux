/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2015 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <sched.h>	// for CPU_COUNT(3), CPU_SETSIZE, CPU_ISSET(3)
#include <unistd.h>	// for sysconf(3)
#include <trace_utils.h>// for TRACE()
#include <err_utils.h>	// for CHECK_ZERO_ERRNO(), CHECK_NOT_M1()
#include <cpu_set_utils.h>	// for cpu_set_print()

/*
 * This shows how to create threads with a certain affinity.
 *
 * Notes:
 * - Affinity in Linux is to a *group* of CPUs.
 * - If the group contains just one CPU then you dictate the CPU on which
 * the thread or process will run.
 * - You can see the affinity of a process [pid] this way:
 * cat /proc/[pid]/status | grep Cpus_allowed
 * - Threads and processes inherit the affinity of their parents and cannot
 * extend that affinity (unless they are root or have special privileges).
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

void *worker(void *p) {
	int num=*(int *)p;
	//TRACE("starting thread [%d]", num);
	int cpunum=CHECK_NOT_M1(sched_getcpu());
	TRACE("thread [%d] running on cpu [%d]", num, cpunum);
	//TRACE("ending thread [%d]", num);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	const int cpu_num=CHECK_NOT_M1(sysconf(_SC_NPROCESSORS_ONLN));
	const int num=cpu_num;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	cpu_set_t cpu_sets[num];
	int ids[num];

	//TRACE("main starting");
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		//cpu_set_print(cpu_sets + i);
		CHECK_ZERO_ERRNO(pthread_attr_init(attrs + i));
		CHECK_ZERO_ERRNO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO_ERRNO(pthread_create(threads + i, attrs + i, worker, ids + i));
	}
	//TRACE("finished creating threads");
	for(int i=0; i<num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], NULL));
	}
	//TRACE("main ended");
	return EXIT_SUCCESS;
}
