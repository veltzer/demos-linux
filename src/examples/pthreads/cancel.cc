/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<pthread.h> // for pthread_setcancelstate(3)
#include<unistd.h> // for sleep(3)

#include<us_helper.h> // for CHECK_ZERO(), TRACE()

/*
 * This demo is a pthread_cancel demo and was copied from the pthread_cancel
 * manpage.
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
