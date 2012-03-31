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

#include<pthread.h>
#include<sched.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS
#include<unistd.h> // for sysconf(3)

#include"us_helper.h"

/*
 * This shows how to create threads with a certain affinity
 *
 * EXTRA_LIBS=-lpthread
 */
void print_cpu_set(cpu_set_t *p) {
	fprintf(stderr, "CPU_COUNT is %d\n", CPU_COUNT(p));
	fprintf(stderr, "CPU_SETSIZE is %d\n", CPU_SETSIZE);
	for (int j = 0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, p)) {
			printf("\tCPU %d\n", j);
		}
	}
}

void *worker(void *p) {
	int num = *(int *)p;
	fprintf(stderr, "starting thread %d\n", num);
	fprintf(stderr, "ending thread %d\n", num);
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
	const int num = 10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	cpu_set_t cpu_sets[num];
	int ids[num];
	void* rets[num];

	fprintf(stderr, "main starting\n");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		print_cpu_set(cpu_sets + i);
		SCIG(pthread_attr_init(attrs + i), "pthread_attr_init");
		SCIG(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i), "pthread_attr_setaffinity_np");
		SCIG(pthread_create(threads + i, attrs + i, worker, ids + i), "pthread_create");
	}
	fprintf(stderr, "main ended creating threads\n");
	for (int i = 0; i < num; i++) {
		SCIG(pthread_join(threads[i], rets + i), "pthread_join");
	}
	fprintf(stderr, "main ended\n");
	return EXIT_SUCCESS;
}
