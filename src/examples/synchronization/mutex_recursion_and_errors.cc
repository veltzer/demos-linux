#include <stdio.h> // for printf(3)
#include <sys/time.h> // for gettimeofday(2)
#include <pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3), pthread_mutex_init(3), pthread_mutex_destroy(3)
#include <semaphore.h> // for sem_init(3), sem_wait(3), sem_post(3)
#include <sys/types.h> // for ftok(3), semget(3), semctl(3), semop(3)
#include <sys/ipc.h> // for ftok(3), semget(3), semctl(3), semop(3)
#include <sys/sem.h> // for semget(3), semctl(3), semop(3)

#include "us_helper.hh" // for micro_diff, CHECK_ZERO

/*
 * This demo shows that the default pthread_mutex is non recursive, it boggs you in an
 * infinite look. This is the FAST kind of mutex that the pthread_mutex documentation
 * is talking about.
 *
 * Note: YOU MUST compile with -lpthread or you get very weird results with no error
 * checking for the locks.
 *
 * Results:
 * - destroying a lock that is already locked is an error. This is true for ANY
 *   type of lock.
 * - the default lock is indeed blocking if re acquired by the same thread.
 *   That is also why it is a little bit faster than regular mutexes (see the demo
 *   that shows the timings of various mutexes).
 * - the recusive lock does seem to count the number of times the lock is taken. You
 *   can see this by acquiring the lock 2 times, releasing it once and seeing that the
 *   destroy function is creating an error (knowing that the lock is still taken one
 *   time).
 * 
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 * - make this example take command line arguments about which type of lock to create
 *   (fast, errorcheck or recursive) and which type of error to create
 *   (double acuisition, unlock without lock or destroy without unlock)
 * - investigate recursive mutexes and show that they indeed keep holding the lock
 *   after the following: lock, lock, unlock. The documentation seems to say that
 *   they do (count the number of lock and unlocks).
 *
 */

int main(int argc, char **argv, char **envp) {
	printf("main started\n");
	// this is the default type of locked (the "FAST" kind...)
	//pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
	//pthread_mutex_t mutex=PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
	pthread_mutex_t mutex=PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	//CHECK_ZERO(pthread_mutex_init(&mutex,NULL));
	CHECK_ZERO(pthread_mutex_lock(&mutex));
	CHECK_ZERO(pthread_mutex_lock(&mutex));
	CHECK_ZERO(pthread_mutex_unlock(&mutex));
	CHECK_ZERO(pthread_mutex_unlock(&mutex));
	CHECK_ZERO(pthread_mutex_destroy(&mutex));
	printf("main ended\n");
	return(0);
}
