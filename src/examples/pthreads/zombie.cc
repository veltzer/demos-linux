/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <pthread.h>	// for pthread_create(3)
#include <string.h>	// for strncpy(3)
#include <sys/prctl.h>	// for prctl(2)
#include <unistd.h>	// for sleep(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_ZERO(), TRACE()

/*
 * This example creates a zombie thread.
 * definition: A zombie thread is a joinable thread which is not joined by the
 * parent.
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
 * EXTRA_LINK_FLAGS=-lpthread
 */
void set_thread_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name, newname, size);
	CHECK_NOT_M1(prctl(PR_SET_NAME, name));
}

pthread_mutex_t mutex_init, mutex_start, mutex_end;
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

int main(int argc, char** argv, char** envp) {
	set_thread_name("parent");
	pthread_t thread;

	TRACE("main started");
	CHECK_ZERO(pthread_mutex_init(&mutex_init, NULL));
	CHECK_ZERO(pthread_mutex_init(&mutex_start, NULL));
	CHECK_ZERO(pthread_mutex_init(&mutex_end, NULL));
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
	CHECK_ZERO(sleep(1));
	// CHECK_ZERO(pthread_join(thread,NULL));

	// now the thread is dead, lets print it's state (same as before...)
	my_system("ps --no-headers -L -p %d -o pid,lwp,comm", getpid());
	TRACE("main ended");
	return EXIT_SUCCESS;
}
