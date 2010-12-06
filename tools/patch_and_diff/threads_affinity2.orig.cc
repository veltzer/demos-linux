//#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// for sysconf
#include <unistd.h>

#include "us_helper.h"

/*
 *      This shows how to create threads with a certain affinity
 */
void another_func() {
	printf("Hello, World!");
}


void print_cpu_set(cpu_set_t *p) {
	fprintf(stderr, "CPU_COUNT is %d\n", CPU_COUNT(p));
	fprintf(stderr, "CPU_SETSIZE is %d\n", CPU_SETSIZE);
	for (int j = 0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, p)) {
			printf("    CPU %d\n", j);
		}
	}
}


void *worker(void *p) {
	int num = *(int *)p;

	fprintf(stderr, "starting thread %d\n", num);
	fprintf(stderr, "ending thread %d\n", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
	const int num = 10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	cpu_set_t cpu_sets[num];
	int ids[num];
	void           *rets[num];

	fprintf(stderr, "main starting\n");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		print_cpu_set(cpu_sets + i);
		SC0(pthread_attr_init(attrs + i), "pthread_attr_init");
		SC0(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i), "pthread_attr_setaffinity_np");
		SC0(pthread_create(threads + i, attrs + i, worker, ids + i), "pthread_create");
	}
	fprintf(stderr, "main ended creating threads\n");
	for (int i = 0; i < num; i++) {
		SC0(pthread_join(threads[i], rets + i), "pthread_join");
	}
	fprintf(stderr, "main ended\n");
	return(0);
}
