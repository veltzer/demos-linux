#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 *      This is a demo of how to use a cpu set
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
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


int main(int argc, char **argv, char **envp) {
	fprintf(stderr, "number of cpus is %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	for (int i = 0; i < 500; i += 100) {
		CPU_SET(i, &cpuset);
	}
	print_cpu_set(&cpuset);
	return(0);
}
