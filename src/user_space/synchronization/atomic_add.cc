#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h> // for sysconf

#include "us_helper.hh"

/*
 *      This is a demo which shows atomic add using the
 *      __sync_add_and_fetch gcc function.
 *      see
 *      http://gcc.gnu.org/onlinedocs/gcc/Atomic-Builtins.html
 *      for more details...
 *
 *      The idea here is to try to break this function by having multiple
 *      thread try to increment the counter at once. This is achieved by
 *      using pthread barriers so this example is based on the pthread barriers example.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
pthread_barrier_t bar;
int counter = 0;
FILE              *pfile = stdout;
const int wait_usecs = 0;
//FILE* pfile=stderr;
void print_cpu_set(cpu_set_t *p) {
	fprintf(pfile, "_SC_NRPROCESSORS_ONLN is %d\n", cpu_num);
	fprintf(pfile, "CPU_COUNT is %d\n", CPU_COUNT(p));
	fprintf(pfile, "CPU_SETSIZE is %d\n", CPU_SETSIZE);
	for (int j = 0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, p)) {
			printf("\tCPU %d\n", j);
		}
	}
}


void *worker(void *p) {
	int num = *(int *)p;

	fprintf(pfile, "starting thread %d\n", num);
	for (int i = 0; i < 10; i++) {
		int ret;
		SCIG2(ret = pthread_barrier_wait(&bar), "pthread_barrier_wait", 0, PTHREAD_BARRIER_SERIAL_THREAD);
		//fprintf(pfile, "thread %d got %d from pthread_barrier_wait\n", num, ret);
		//int b = __sync_add_and_fetch(&counter, 1);
		__sync_add_and_fetch(&counter, 1);
		//fprintf(pfile, "thread %d got %d from counter\n", num, b);
		usleep(wait_usecs);
		fprintf(pfile, "thread %d finished work\n", num);
	}
	fprintf(pfile, "ending thread %d\n", num);
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	const int num = 10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	int ids[num];
	cpu_set_t cpu_sets[num];
	void           *rets[num];

	SCIG(pthread_barrier_init(&bar, NULL, num), "pthread_barrier_init");
	fprintf(pfile, "main starting\n");
	for (int i = 0; i < num; i++) {
		ids[i] = i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		print_cpu_set(cpu_sets + i);
		SCIG(pthread_attr_init(attrs + i), "pthread_attr_init");
		SCIG(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i), "pthread_attr_setaffinity_np");
		SCIG(pthread_create(threads + i, attrs + i, worker, ids + i), "pthread_create");
	}
	fprintf(pfile, "main ended creating threads\n");
	for (int i = 0; i < num; i++) {
		SCIG(pthread_join(threads[i], rets + i), "pthread_join");
	}
	SCIG(pthread_barrier_destroy(&bar), "pthread_barrier_destroy");
	fprintf(pfile, "main ended\n");
	return(0);
}
