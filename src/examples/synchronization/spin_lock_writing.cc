#include <stdio.h> // for fprintf(3)
#include <pthread.h> // for pthread_create(3), pthread_join(3), pthread_spin_init(3), pthread_spin_destroy(3)
	// pthread_spin_lock(3), pthread_spin_unlock(3), pthread_attr_init(3), pthread_attr_setaffinity_np(3)
#include <unistd.h> // for sysconf(3)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE // needed for SCHED_IDLE, SCHED_BATCH
#endif // _GNU_SOURCE
#include <sched.h> // for CPU_ZERO(3), CPU_SET(3)

#include "us_helper.hh"

/*
 * This is an example of writing your own spin locks...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
// this is the spin lock implementation (pthread "like")
typedef struct _mypthread_spinlock_t {
	int val;
} mypthread_spinlock_t;

int mypthread_spin_init(mypthread_spinlock_t* lock) {
	lock->val=0;
	return 0;
}
int mypthread_spin_destroy(mypthread_spinlock_t* lock) {
	// do nothing
	return 0;
}
int mypthread_spin_lock(mypthread_spinlock_t* lock) {
	// lets spin!...
	while(!__sync_bool_compare_and_swap(&(lock->val),0,1)) {
		// on platform where atomic ops are NOT compiler
		// barrier you need to add a compiler barrier on
		// lock->val right here inside the loop...
	}
	return 0;
}
int mypthread_spin_unlock(mypthread_spinlock_t* lock) {
	__sync_bool_compare_and_swap(&(lock->val),1,0);
	return 0;
}

FILE* pfile=stderr;
const int loops=3;
mypthread_spinlock_t lock;
int counter=0;
const int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);

void *worker(void *p) {
	int num = *(int *)p;
	fprintf(pfile, "starting thread %d\n", num);
	int success=0;
	while(success<loops) {
		CHECK_ZERO(mypthread_spin_lock(&lock));
		if(counter%cpu_num==num) {
			fprintf(pfile, "thread %d caught lock\n", num);
			sleep(1);
			counter++;
			success++;
			fprintf(pfile, "thread %d released lock\n", num);
		}
		CHECK_ZERO(mypthread_spin_unlock(&lock));
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
	CHECK_ZERO(mypthread_spin_init(&lock));
	for (int i = 0; i < thread_num; i++) {
		ids[i] = i;
		CPU_ZERO(cpu_sets + i);
		CPU_SET(i % cpu_num, cpu_sets + i);
		CHECK_ZERO(pthread_attr_init(attrs + i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs + i, sizeof(cpu_set_t), cpu_sets + i));
		CHECK_ZERO(pthread_create(threads + i, attrs + i, worker, ids + i));
	}
	fprintf(pfile, "main ended creating threads\n");
	for (int i = 0; i < thread_num; i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	CHECK_ZERO(mypthread_spin_destroy(&lock));
	delete threads;
	delete attrs;
	delete ids;
	delete cpu_sets;
	fprintf(pfile, "counter is %d\n",counter);
	fprintf(pfile, "main ended\n");
	return(0);
}
