/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3), exit(3)
#include <pthread.h>	// for pthread_t, pthread_attr_t, pthread_create(3), pthread_join(3)
#include <unistd.h>	// for sysconf(3), usleep(3), getopt_long(3)
#include <sched.h>	// for cpu_set_t, CPU_ZERO(3), CPU_SET(3), sched_getcpu(2)
#include <getopt.h>	// for struct option
#include <us_helper.h>	// for CHECK_ZERO(), CHECK_ONEOFTWO(), TRACE(), print_cpu_set()

/*
 * This is a demo which shows atomic add using the
 * __sync_add_and_fetch gcc function.
 * see
 * http://gcc.gnu.org/onlinedocs/gcc/Atomic-Builtins.html
 * for more details...
 *
 * The idea here is to try to break this function by having multiple
 * thread try to increment the value at once.
 *
 * Notes:
 * - We see that if we run the threads and have them do regular +=1 instead of
 * atomic ops we get really bad values for the shared counter.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 *
 * TODO:
 * - the results of this example don't add up. Some of these __sync_add_and_fetch
 * calls must fail because other CPUs are doing them at the same time. How come they don't?
 * Are they guaranteed to succeed according to the Intel manual? It looks that way. Investigate.
 */

// data to be passed to each thread...
typedef struct _thread_data {
	int num;
	int attempts;
	int* value;
	int usleep_interval;
} thread_data;

static void *regular_worker(void *p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	for(int i=0; i<td->attempts; i++) {
		*(td->value)+=1;
	}
	TRACE("end thread %d", td->num);
	return(NULL);
}
static void *compiler_barrier_worker(void* p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	for(int i=0; i<td->attempts; i++) {
		*(td->value)+=1;
		asm volatile ("" ::: "memory");
	}
	TRACE("end thread %d", td->num);
	return(NULL);
}
static void *atomic_worker(void *p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	for(int i=0; i<td->attempts; i++) {
		__sync_add_and_fetch(td->value, 1);
	}
	TRACE("end thread %d", td->num);
	return(NULL);
}
static void *observer(void *p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	while(true) {
		CHECK_NOT_M1(usleep(td->usleep_interval));
		TRACE("value is %d", *(td->value));
	}
	TRACE("end thread %d", td->num);
	return(NULL);
}

static int parse_arguments(int& argc, char** argv, bool& doObserver, int& type, int& attempts) {
	while(true) {
		int option_index=0;
		static struct option long_options[]={
			{"observer", no_argument, 0, 0},
			{"type", required_argument, 0, 1},
			{"attempts", required_argument, 0, 2},
			{0, 0, 0, 0}
		};
		int c=getopt_long(argc, argv, "", long_options, &option_index);
		if (c==-1)
			break;
		switch (c) {
		// observer
		case 0:
			doObserver=true;
			break;
		// type
		case 1:
			type=atoi(optarg);
			break;
		// attempts
		case 2:
			attempts=atoi(optarg);
			break;
		default:
			fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
		}
	}
	if(argc-optind<1) {
		fprintf(stderr, "%s: too few threads\n", argv[0]);
		fprintf(stderr, "%s: usage: %s [options] [core] [core] [core..]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: select type of threads using --type=[argument]\n", argv[0]);
		fprintf(stderr, "%s:\ttype=0 means regular threads\n", argv[0]);
		fprintf(stderr, "%s:\ttype=1 means compiler barrier threads\n", argv[0]);
		fprintf(stderr, "%s:\ttype=2 means atomic ops threads threads (default)\n", argv[0]);
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "running with doObserver %d\n", doObserver);
		fprintf(stderr, "running with type %d\n", type);
		fprintf(stderr, "running with attempts %d\n", attempts);
	}
	return optind;
}
int main(int argc, char** argv, char** envp) {
	bool doObserver=false;
	int type=2;	// atomic
	int attempts=100000;
	int optind=parse_arguments(argc, argv, doObserver, type, attempts);
	// find the number of cores
	const int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);
	// the value to be incremented by all threads
	int value=0;
	const int usleep_interval=10000;
	const int real_threads=argc-optind;
	const int thread_num=doObserver ? real_threads+1 : real_threads;
	TRACE("cpu_num is %d", cpu_num);
	TRACE("attempts is %d", attempts);
	TRACE("thread_num is %d", thread_num);
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	thread_data* data=new thread_data[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];
	void** rets=new void*[thread_num];

	TRACE("start creating threads");
	for(int i=0; i<thread_num; i++) {
		data[i].num=i;
		data[i].attempts=attempts;
		data[i].value=&value;
		data[i].usleep_interval=usleep_interval;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(atoi(argv[optind+i]), cpu_sets+i);
		// print_cpu_set(stderr,cpu_sets + i);
		CHECK_ZERO(pthread_attr_init(attrs + i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		if(i==thread_num-1 && doObserver) {
			CHECK_ZERO(pthread_create(threads + i, attrs + i, observer, data + i));
		} else {
			switch(type) {
			case 0:
				CHECK_ZERO(pthread_create(threads + i, attrs + i, regular_worker, data + i));
				break;
			case 1:
				CHECK_ZERO(pthread_create(threads + i, attrs + i, compiler_barrier_worker, data + i));
				break;
			case 2:
				CHECK_ZERO(pthread_create(threads + i, attrs + i, atomic_worker, data + i));
				break;
			default:
				fprintf(stderr, "bad type of thread (%d)\n", type);
				exit(EXIT_FAILURE);
			}
		}
	}
	TRACE("created threads");
	TRACE("joining threads");
	for(int i=0; i<real_threads; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets+i));
	}
	TRACE("joined threads");
	TRACE("value is %d and should be %d", value, real_threads*attempts);
	TRACE("ended");
	return EXIT_SUCCESS;
}
