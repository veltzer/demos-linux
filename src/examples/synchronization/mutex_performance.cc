#include<stdio.h> // for printf(3)
#include<sys/time.h> // for gettimeofday(2)
#include<pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_init(3), pthread_mutex_destory(3)
#include<semaphore.h> // for sem_init(3), sem_wait(3), sem_post(3)
#include<sys/types.h> // for ftok(3), semget(3), semctl(3), semop(3)
#include<sys/ipc.h> // for ftok(3), semget(3), semctl(3), semop(3)
#include<sys/sem.h> // for semget(3), semctl(3), semop(3)

#include"us_helper.hh" // for micro_diff, CHECK_ZERO

/*
 * This demo shows the difference between regular pthread mutex (which is a
 * futex) and an expensive one.
 *
 * The idea is a single thread application that just measures a million lock/unlock
 * operations on each type of lock. Simple and effective.
 *
 * We measure ALL kinds of semaphores and mutexes here.
 *
 * Results:
 * the main finding is that
 * regular lock/unlock costs about 90 nanos on modern hardware while SYSV IPC costs
 * 10 times more. In all other aspects all other types of locks (recursive, non
 * recursive, shared, non shared) perform about the same.
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 */

void measure(pthread_mutex_t* mutex,sem_t* sem, int semid,const char* name) {
	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("running test [%s]\n",name);
	gettimeofday(&t1, NULL);
	for(unsigned int i = 0;i < loop;i++) {
		if(mutex) {
			CHECK_ZERO(pthread_mutex_lock(mutex));
			CHECK_ZERO(pthread_mutex_unlock(mutex));
		}
		if(sem) {
			CHECK_ZERO(sem_wait(sem));
			CHECK_ZERO(sem_post(sem));
		}
		if(semid!=-1) {
			struct sembuf sops;
			sops.sem_num = 0;
			sops.sem_op = 1;
			sops.sem_flg = 0;
			CHECK_NOT_M1(semop(semid, &sops, 1));
			sops.sem_num = 0;
			sops.sem_op = -1;
			sops.sem_flg = 0;
			CHECK_NOT_M1(semop(semid, &sops, 1));
		}
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one lock/unlock pair: %lf\n", micro_diff(&t1,&t2)/(double)loop);
}

static pthread_mutex_t mutex_fast;
static pthread_mutex_t mutex_recursive;
static pthread_mutex_t mutex_errorcheck;
static sem_t sem_nonshared;
static sem_t sem_shared;
static int semid;

void* work(void* param) {
	measure(&mutex_fast,NULL,-1,"fast mutexes");
	measure(&mutex_recursive,NULL,-1,"recursive mutexes");
	measure(&mutex_errorcheck,NULL,-1,"error checking mutexes");
	measure(NULL,&sem_nonshared,-1,"non shared semaphores");
	measure(NULL,&sem_shared,-1,"shared semaphores");
	measure(NULL,NULL,semid,"SYSV IPC semaphores");
	return NULL;
}

int main(int argc, char **argv, char **envp) {
	printf("main started\n");
	pthread_mutexattr_t attr;
	key_t key;
	CHECK_NOT_M1(key = ftok("/etc/passwd", 'x'));
	CHECK_NOT_M1(semid = semget(key, 1, IPC_CREAT | 0666));
	CHECK_NOT_M1(semctl(semid,0,SETVAL,1));

	CHECK_ZERO(sem_init(&sem_nonshared,0,1));
	CHECK_ZERO(sem_init(&sem_shared,1,1));
	CHECK_ZERO(pthread_mutexattr_init(&attr));
	CHECK_ZERO(pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_FAST_NP));
	CHECK_ZERO(pthread_mutex_init(&mutex_fast,&attr));
	CHECK_ZERO(pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP));
	CHECK_ZERO(pthread_mutex_init(&mutex_recursive,&attr));
	CHECK_ZERO(pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK_NP));
	CHECK_ZERO(pthread_mutex_init(&mutex_errorcheck,&attr));
	run_high_priority(work,NULL,90);
	CHECK_ZERO(pthread_mutex_destroy(&mutex_fast));
	CHECK_ZERO(pthread_mutex_destroy(&mutex_recursive));
	CHECK_ZERO(pthread_mutex_destroy(&mutex_errorcheck));
	return EXIT_SUCCESS;
}
