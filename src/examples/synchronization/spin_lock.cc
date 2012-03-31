#include<stdio.h> // for fprintf(3)
#include<pthread.h> // for pthread_create(3), pthread_join(3), pthread_spin_init(3), pthread_spin_destroy(3)
	// pthread_spin_lock(3), pthread_spin_unlock(3), pthread_attr_init(3), pthread_attr_setaffinity_np(3)
#include<unistd.h> // for sysconf(3)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE // needed for SCHED_IDLE, SCHED_BATCH
#endif // _GNU_SOURCE
#include<sched.h> // for CPU_ZERO(3), CPU_SET(3)

#include"us_helper.hh"

/*
 * This is a demo for using pthread spin locks...
 *
 *	Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
FILE* pfile=stderr;
const int loops=3;
pthread_spinlock_t lock;
int counter=0;
const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);

void *worker(void *p) {
	int num = *(int *)p;
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


int main(int argc, char **argv, char **envp) {
	const int thread_num=cpu_num;
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	int* ids=new int[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];

	fprintf(pfile, "main starting\n");
	CHECK_ZERO(pthread_spin_init(&lock,PTHREAD_PROCESS_PRIVATE));
	for(int i=0;i<thread_num;i++) {
		ids[i]=i;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(i%cpu_num,cpu_sets+i);
		CHECK_ZERO(pthread_attr_init(attrs+i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs+i,sizeof(cpu_set_t),cpu_sets+i));
		CHECK_ZERO(pthread_create(threads+i,attrs+i,worker,ids+i));
	}
	fprintf(pfile, "main ended creating threads\n");
	for(int i=0;i<thread_num;i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	CHECK_ZERO(pthread_spin_destroy(&lock));
	delete threads;
	delete attrs;
	delete ids;
	delete cpu_sets;
	fprintf(pfile, "counter is %d\n",counter);
	fprintf(pfile, "main ended\n");
	return EXIT_SUCCESS;
}
