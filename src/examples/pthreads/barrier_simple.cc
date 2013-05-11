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
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, rand(3)
#include <pthread.h>	// for pthread_t, pthread_attr_t, pthread_barrier_t, pthread_create(3)
#include <unistd.h>	// for sysconf(3), usleep(3), sleep(3)
#include <sched.h>	// for cpu_set_t, CPU_ZERO(3), CPU_SET(3), sched_getcpu(3)
#include <us_helper.h>	// for CHECK_ZERO(), CHECK_ONEOFTWO(), INFO(), CHECK_NOT_M1(), print_cpu_set()

/*
 * This demo shows off pthread barriers which are a way to synchronize a number of threads.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

typedef struct _thread_data {
	int num;
	pthread_barrier_t* barrier;
	int attempts;
	int max_sleep_time;
	int min_sleep_time;
} thread_data;

static void *worker(void *p) {
	thread_data* td=(thread_data*)p;
	INFO("start thread (%d), on cpu %d", td->num, sched_getcpu());
	for(int i=0; i<td->attempts; i++) {
		// block on the barrier
		CHECK_ONEOFTWO(pthread_barrier_wait(td->barrier), 0, PTHREAD_BARRIER_SERIAL_THREAD);
		// give a chance for other thread to print that they stopped sleeping (hack)
		// CHECK_NOT_M1(usleep(1000));
		INFO("doing work (%d)", td->num);
		// sleep for a random time
		sleep(rand()%td->max_sleep_time+td->min_sleep_time);
		INFO("stopped_sleeping (%d)", td->num);
	}
	INFO("end thread %d", td->num);
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	if(argc!=1) {
		fprintf(stderr, "%s: usage: %s\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const int thread_num=10;
	const int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);
	const int attempts=10;
	const int max_sleep_time=10;
	const int min_sleep_time=2;
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	thread_data* data=new thread_data[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];
	void** rets=new void*[thread_num];
	pthread_barrier_t bar;

	CHECK_ZERO(pthread_barrier_init(&bar, NULL, thread_num));
	INFO("start creating threads");
	for(int i=0; i<thread_num; i++) {
		data[i].num=i;
		data[i].attempts=attempts;
		data[i].barrier=&bar;
		data[i].max_sleep_time=max_sleep_time;
		data[i].min_sleep_time=min_sleep_time;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(i%cpu_num, cpu_sets+i);
		// print_cpu_set(stderr,cpu_sets + i);
		CHECK_ZERO(pthread_attr_init(attrs + i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO(pthread_create(threads + i, attrs + i, worker, data + i));
	}
	// give the threads a chance to print their start messages
	CHECK_NOT_M1(usleep(1000));
	INFO("created threads");
	INFO("joining threads");
	for(int i=0; i<thread_num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets+i));
	}
	INFO("joined threads");
	CHECK_ZERO(pthread_barrier_destroy(&bar));
	INFO("ended");
	return EXIT_SUCCESS;
}
