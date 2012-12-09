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

#include <firstinclude.h>
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <pthread.h> // for pthread_t, pthread_attr_t, pthread_create(3), pthread_join(3)
#include <unistd.h> // for sysconf(3)
#include <sched.h> // for cpu_set_t, CPU_ZERO(3), CPU_SET(3), sched_getcpu(2)
#include <us_helper.h> // for CHECK_ZERO(), CHECK_ONEOFTWO(), TRACE(), print_cpu_set()

/*
* This is a demo which shows atomic add using the
* __sync_add_and_fetch gcc function.
* see
* http://gcc.gnu.org/onlinedocs/gcc/Atomic-Builtins.html
* for more details...
*
* The idea here is to try to break this function by having multiple
* thread try to increment the counter at once.
*
* EXTRA_LIBS=-lpthread
*
* TODO:
* 	- have each thread print on which cpu it is currently running.
* 	- the results of this example don't add up. Some of these __sync_add_and_fetch
* 	calls must fail because other CPUs are doing them at the same time. How come they don't?
* 	Are they guaranteed to succeed according to the Intel manual? It looks that way. Investigate.
*/

// data to be passed to each thread...
typedef struct _thread_data {
	int num;
	int attempts;
	int* value;
} thread_data;

static void *worker(void *p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d",td->num,sched_getcpu());
	for(int i=0;i<td->attempts;i++) {
		__sync_add_and_fetch(td->value, 1);
	}
	TRACE("end thread %d",td->num);
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	if(argc<3) {
		fprintf(stderr,"%s: usage: %s [attempts] [core] [core] [core..]\n",argv[0],argv[0]);
		return EXIT_FAILURE;
	}
	// find the number of cores
	const int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);
	// the counter to be incremented by all threads
	int counter=0;
	int attempts=atoi(argv[1]);
	const int thread_num=argc-2;
	TRACE("cpu_num is %d",cpu_num);
	TRACE("attempts is %d",attempts);
	TRACE("thread_num is %d",thread_num);
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	thread_data* data=new thread_data[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];
	void** rets=new void*[thread_num];

	TRACE("start creating threads");
	for(int i=0;i<thread_num;i++) {
		data[i].num=i;
		data[i].attempts=attempts;
		data[i].value=&counter;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(i%cpu_num,cpu_sets+i);
		//print_cpu_set(stderr,cpu_sets + i);
		CHECK_ZERO(pthread_attr_init(attrs + i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO(pthread_create(threads + i, attrs + i, worker, data + i));
	}
	TRACE("created threads");
	TRACE("joining threads");
	for(int i=0;i<thread_num;i++) {
		CHECK_ZERO(pthread_join(threads[i],rets+i));
	}
	TRACE("joined threads");
	TRACE("counter is %d and should be %d",counter,thread_num*attempts);
	TRACE("ended");
	return EXIT_SUCCESS;
}
