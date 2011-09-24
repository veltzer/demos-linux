#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * This demo is a pthread_cancel demo and was copied from the pthread_cancel
 * manpage.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 * - make it my own (with my own wrappers, less code and more demo).
 */

#define handle_error_en(en, msg) \
       do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void * thread_func(void *ignored_argument) {
	int s;
	/* Disable cancellation for a while, so that we don't
	*               immediately react to a cancellation request */
	s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_setcancelstate");
	printf("thread_func(): started; cancellation disabled\n");
	sleep(5);
	printf("thread_func(): about to enable cancellation\n");
	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_setcancelstate");
	/* sleep() is a cancellation point */
	sleep(1000);        /* Should get canceled while we sleep */
	/* Should never get here */
	printf("thread_func(): not canceled!\n");
	return NULL;
}

int main(void) {
	pthread_t thr;
	void *res;
	int s;

	/* Start a thread and then send it a cancellation request */

	s = pthread_create(&thr, NULL, &thread_func, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_create");
	sleep(2); /* Give thread a chance to get started */
	printf("main(): sending cancellation request\n");
	s = pthread_cancel(thr);
	if (s != 0)
		handle_error_en(s, "pthread_cancel");
	/* Join with thread to see what its exit status was */
	s = pthread_join(thr, &res);
	if (s != 0)
		handle_error_en(s, "pthread_join");
	if (res == PTHREAD_CANCELED)
		printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled (shouldn't happen!)\n");
}
