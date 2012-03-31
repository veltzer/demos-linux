#include<pthread.h> // for pthread_create(3), pthread_join(3)
#include<sched.h> // for CPU_COUNT(3), CPU_SETSIZE, CPU_ISSET(3)
#include<unistd.h> // for sysconf(3)

#include"us_helper.hh" // for TRACE(), CHECK_ZERO()

/*
 * This shows how to create threads with a certain affinity
 *
 *	Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void print_cpu_set(cpu_set_t *p) {
	TRACE("CPU_COUNT is %d", CPU_COUNT(p));
	TRACE("CPU_SETSIZE is %d", CPU_SETSIZE);
	for(int j=0;j<CPU_SETSIZE;j++) {
		if (CPU_ISSET(j, p)) {
			TRACE("\tCPU %d", j);
		}
	}
}

void *worker(void *p) {
	int num = *(int *)p;
	TRACE("starting thread %d", num);
	TRACE("ending thread %d", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
	const int num = 10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	cpu_set_t cpu_sets[num];
	int ids[num];

	TRACE("main starting");
	for(int i=0;i<num;i++) {
		ids[i] = i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		print_cpu_set(cpu_sets + i);
		CHECK_ZERO(pthread_attr_init(attrs + i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO(pthread_create(threads + i, attrs + i, worker, ids + i));
	}
	fprintf(stderr,"main ended creating threads\n");
	for(int i=0;i<num;i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	TRACE("main ended");
	return EXIT_SUCCESS;
}
