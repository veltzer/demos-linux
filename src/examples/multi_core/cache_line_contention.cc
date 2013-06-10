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
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3), exit(3), posix_memalign(3)
#include <pthread.h>	// for pthread_t, pthread_attr_t, pthread_create(3), pthread_join(3)
#include <unistd.h>	// for sysconf(3), usleep(3), getopt_long(3)
#include <sched.h>	// for cpu_set_t, CPU_ZERO(3), CPU_SET(3), sched_getcpu(2)
#include <getopt.h>	// for struct option
#include <us_helper.h>	// for CHECK_ZERO_ERRNO(), CHECK_NOT_M1(), TRACE(), print_cpu_set(), micro_diff()

/*
 * This demo shows the difference in speed of running two threads using the same cache line
 * or different cache lines.
 *
 * Results:
 * It seems that the difference on my laptop is about half. Meaning, its faster by on order of 2
 * to run on non cache line shared data.
 *
 * Notes to programmers:
 * - How do we prevent the compiler from making all the loops that we code in the workers
 * go away?
 * The solution is either to attach volatile to both the 'shared' and 'nonshared' members
 * (see below) or to use the 'td->XXXXXXXXX[td->num]+=1;' notation used below (the compiler
 * does not know what 'td->num' is and so actually writes the data.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

// data to be passed to each thread...
typedef struct _thread_data {
	int num;
	unsigned long long attempts;
	int* shared;
	int* nonshared;
	int usleep_interval;
} thread_data;

static void *shared_worker(void *p) {
	thread_data* td=(thread_data*)p;
	// TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	for(unsigned long long i=0; i<td->attempts; i++) {
		td->shared[td->num]+=1;
	}
	// TRACE("end thread %d", td->num);
	return(NULL);
}
static void *nonshared_worker(void* p) {
	thread_data* td=(thread_data*)p;
	// TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	for(unsigned long long i=0; i<td->attempts; i++) {
		td->nonshared[td->num]+=1;
	}
	// TRACE("end thread %d", td->num);
	return(NULL);
}
static void *observer(void *p) {
	thread_data* td=(thread_data*)p;
	TRACE("start thread %d, running on core %d", td->num, sched_getcpu());
	while(true) {
		CHECK_NOT_M1(usleep(td->usleep_interval));
		// TRACE("value is %d", *(td->value));
	}
	TRACE("end thread %d", td->num);
	return(NULL);
}

static int parse_arguments(int& argc, char** argv, bool& doObserver, int& type, unsigned long long& attempts) {
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
			attempts=atoll(optarg);
			break;
		default:
			fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
			break;
		}
	}
	if(argc-optind<1) {
		fprintf(stderr, "%s: too few threads\n", argv[0]);
		fprintf(stderr, "%s: usage: %s [options] [core] [core] [core..]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: select type of threads using --type=[argument]\n", argv[0]);
		fprintf(stderr, "%s:\ttype=0 means shared thread (default)\n", argv[0]);
		fprintf(stderr, "%s:\ttype=1 means nonshared threads\n", argv[0]);
		fprintf(stderr, "%s: select attempts using --attempts=[argument]\n", argv[0]);
		fprintf(stderr, "%s: for example: --type=0 --attempts=1000000 0 1 2 3\n", argv[0]);
		exit(EXIT_FAILURE);
	} else {
		fprintf(stderr, "running with doObserver %d\n", doObserver);
		fprintf(stderr, "running with type %d\n", type);
		fprintf(stderr, "running with attempts %llu\n", attempts);
	}
	return optind;
}
// allocate a single cache line
void* malloc_one_cache_line() {
	void* ptr;
	unsigned int cache_line_size=CHECK_NOT_M1(sysconf(_SC_LEVEL2_CACHE_LINESIZE));
	CHECK_ZERO(posix_memalign(&ptr, cache_line_size, cache_line_size));
	return ptr;
}
int main(int argc, char** argv, char** envp) {
	bool doObserver=false;
	int type=0;	// atomic
	unsigned long long attempts=1000000000;
	int optind=parse_arguments(argc, argv, doObserver, type, attempts);
	// find the number of cores
	const int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);
	const int usleep_interval=10000;
	const int real_threads=argc-optind;
	const int thread_num=doObserver ? real_threads+1 : real_threads;
	TRACE("cpu_num is %d", cpu_num);
	TRACE("attempts is %llu", attempts);
	TRACE("thread_num is %d", thread_num);
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	thread_data* data=new thread_data[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];
	void** rets=new void*[thread_num];

	int *shared=(int*)malloc_one_cache_line();

	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	// TRACE("start creating threads");
	for(int i=0; i<thread_num; i++) {
		data[i].num=i;
		data[i].attempts=attempts;
		data[i].nonshared=(int*)malloc_one_cache_line();
		data[i].shared=shared;
		data[i].usleep_interval=usleep_interval;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(atoi(argv[optind+i]), cpu_sets+i);
		// print_cpu_set(stderr,cpu_sets + i);
		CHECK_ZERO_ERRNO(pthread_attr_init(attrs + i));
		CHECK_ZERO_ERRNO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		if(i==thread_num-1 && doObserver) {
			CHECK_ZERO_ERRNO(pthread_create(threads + i, attrs + i, observer, data + i));
		} else {
			switch(type) {
			case 0:
				CHECK_ZERO_ERRNO(pthread_create(threads + i, attrs + i, shared_worker, data + i));
				break;
			case 1:
				CHECK_ZERO_ERRNO(pthread_create(threads + i, attrs + i, nonshared_worker, data + i));
				break;
			default:
				fprintf(stderr, "bad type of thread (%d)\n", type);
				exit(EXIT_FAILURE);
			}
		}
	}
	// TRACE("created threads");
	// TRACE("joining threads");
	/*
	 * TODO:
	 * - time all of this.
	 */
	for(int i=0; i<real_threads; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], rets+i));
	}
	gettimeofday(&t2, NULL);
	printf("time in micro: %lf\n", micro_diff(&t1, &t2));
	printf("time in micro of a single attempt: %lf\n", micro_diff(&t1, &t2)/(double)attempts);
	// TRACE("joined threads");
	// TRACE("ended");
	return EXIT_SUCCESS;
}
