#include <pthread.h> // for pthread_create
#include <string.h> // for strncpy(3)
#include <sys/prctl.h> // for prctl(2)
#include <unistd.h> // for sleep(3)

#include "us_helper.hh"

/*
 * This example creates a zombie thread.
 * definition: A zombie thread is a thread which is not joined by the parent.
 * This example shows that once this thread dies it does not
 * turn into a zombie process as far as the OS is concerned.
 *
 * So why should you use pthread_join/pthread_detach?
 * In order to clean up the threads memory footprint in the pthread library.
 * You should choose one, and only one, of either pthread_join or pthread_detach.
 *
 * TODO:
 * - show that this example works the same even if you call pthread_detach.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
void set_thread_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name,newname,size);
	CHECK_NOT_M1(prctl(PR_SET_NAME,name));
}

pthread_mutex_t mutex_init,mutex_start,mutex_end;
pid_t tid;

void *worker(void *) {
	TRACE("start");
	set_thread_name("child");
	tid=gettid();
	// lets notify the parent thread that we did initialisation...
	CHECK_ZERO(pthread_mutex_unlock(&mutex_init));
	// wait till the parent tells us to continue...
	CHECK_ZERO(pthread_mutex_lock(&mutex_start));
	// lets notify the parent thread that we're dead...
	CHECK_ZERO(pthread_mutex_unlock(&mutex_end));
	TRACE("end");
	return(NULL);
}

int main(int argc, char **argv, char **envp) {
	set_thread_name("parent");
	pthread_t thread;

	TRACE("main started");
	CHECK_ZERO(pthread_mutex_init(&mutex_init,NULL));
	CHECK_ZERO(pthread_mutex_init(&mutex_start,NULL));
	CHECK_ZERO(pthread_mutex_init(&mutex_end,NULL));
	CHECK_ZERO(pthread_mutex_lock(&mutex_init));
	CHECK_ZERO(pthread_mutex_lock(&mutex_start));
	CHECK_ZERO(pthread_mutex_lock(&mutex_end));

	CHECK_ZERO(pthread_create(&thread, NULL, worker, NULL));
	TRACE("main created thread");
	// wait till the thread has done initialisation
	CHECK_ZERO(pthread_mutex_lock(&mutex_init));
	// ok, the thread is initialized, lets print it's state
	my_system("ps --no-headers -L -p %d -o pid,lwp,comm", getpid());
	// signal the thead to continue...
	CHECK_ZERO(pthread_mutex_unlock(&mutex_start));

	// wait till the thread is over...
	CHECK_ZERO(pthread_mutex_lock(&mutex_end));
	// since the thead called the unlock just before it ended wait just a tiny
	// weeny bit more...
	sleep(1);
	//CHECK_ZERO(pthread_join(thread,NULL));

	// now the thread is dead, lets print it's state (same as before...)
	my_system("ps --no-headers -L -p %d -o pid,lwp,comm", getpid());
	TRACE("main ended");
	return(0);
}
