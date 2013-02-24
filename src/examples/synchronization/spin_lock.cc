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
#include <stdio.h>	// for fprintf(3)
#include <pthread.h>	// for pthread_create(3), pthread_join(3), pthread_spin_init(3), pthread_spin_destroy(3), pthread_spin_lock(3), pthread_spin_unlock(3), pthread_attr_init(3), pthread_attr_setaffinity_np(3)
#include <unistd.h>	// for sysconf(3)
#include <sched.h>	// for CPU_ZERO(3), CPU_SET(3)
#include <us_helper.h>	// for CHECK_ZERO()

/*
 * This is a demo for using pthread spin locks...
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

FILE* pfile=stderr;
const int loops=3;
pthread_spinlock_t lock;
int counter=0;
const int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);

void *worker(void *p) {
	int num=*(int *)p;
	fprintf(pfile, "starting thread %d\n", num);
	int success=0;
	while(success<loops) {
		CHECK_ZERO(pthread_spin_lock(&lock));
		if(counter%cpu_num==num) {
			fprintf(pfile, "thread %d caught lock\n", num);
			sleep(1);
			counter++;
			success++;
			fprintf(pfile, "thread %d released lock\n", num);
		}
		CHECK_ZERO(pthread_spin_unlock(&lock));
	}
	fprintf(pfile, "ending thread %d\n", num);
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	const int thread_num=cpu_num;
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	int* ids=new int[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];

	fprintf(pfile, "main starting\n");
	CHECK_ZERO(pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE));
	for(int i=0; i<thread_num; i++) {
		ids[i]=i;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(i%cpu_num, cpu_sets+i);
		CHECK_ZERO(pthread_attr_init(attrs+i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs+i, sizeof(cpu_set_t), cpu_sets+i));
		CHECK_ZERO(pthread_create(threads+i, attrs+i, worker, ids+i));
	}
	fprintf(pfile, "main ended creating threads\n");
	for(int i=0; i<thread_num; i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	CHECK_ZERO(pthread_spin_destroy(&lock));
	delete threads;
	delete attrs;
	delete ids;
	delete cpu_sets;
	fprintf(pfile, "counter is %d\n", counter);
	fprintf(pfile, "main ended\n");
	return EXIT_SUCCESS;
}
