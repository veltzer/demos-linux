#include<pthread.h> // for pthread_setcancelstate(3)
#include<unistd.h> // for sleep(3)

#include"us_helper.hh" // for CHECK_ZERO(), TRACE()

/*
 * This demo is a pthread_cancel demo and was copied from the pthread_cancel
 * manpage.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 * - show that the cancellation happened in the middle of the sleep and that it was close
 * to the time that asked the cancellation.
 * - show what other functions instead of sleep can be used as cancellation points.
 */

static void * thread_func(void *ignored_argument) {
	/* Disable cancellation for a while, so that we don't
	* immediately react to a cancellation request */
	CHECK_ZERO(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL));
	TRACE("started; cancellation disabled");
	sleep(5);
	TRACE("about to enable cancellation");
	CHECK_ZERO(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL));
	/* sleep() is a cancellation point */
	sleep(1000); /* Should get canceled while we sleep */
	/* Should never get here */
	TRACE("not canceled!");
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	pthread_t thr;
	void *res;

	/* Start a thread and then send it a cancellation request */

	CHECK_ZERO(pthread_create(&thr, NULL, &thread_func, NULL));
	sleep(2); /* Give thread a chance to get started */
	TRACE("sending cancellation request");
	CHECK_ZERO(pthread_cancel(thr));
	/* Join with thread to see what its exit status was */
	CHECK_ZERO(pthread_join(thr, &res));
	if (res == PTHREAD_CANCELED)
		TRACE("thread was canceled");
	else
		TRACE("thread wasn't canceled (shouldn't happen!)");
	return EXIT_SUCCESS;
}
