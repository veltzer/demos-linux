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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <unistd.h>	// for fork(2), _exit(2), sleep(3)
#include <sys/wait.h>	// for waitpid(2)
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <us_helper.h>	// for TRACE(), CHECK_ZERO_ERRNO()

/*
 * EXTRA_LINK_FLAGS=-lpthread
 */

/*
 * This is out thread function for when we'll do multi threading...
 */
void *worker(void *p) {
	TRACE("start");
	CHECK_ZERO(sleep(1));
	TRACE("end");
	return NULL;
}

/*
 * This is an empty testing application
 */
int main(int argc, char** argv, char** envp) {
	TRACE("begin");
	// fork so that we will see if the hooks are called
	// for forked children too (constructors are not called, destructors are...)...
	// this example is for a well behaved child (exists ok...)
	TRACE("forking a well behaved child...");
	pid_t child_pid=fork();
	if(child_pid) {
		// parent
		int status;
		pid_t ret=waitpid(child_pid, &status, 0);
	} else {
		// child
		TRACE("child starting");
		TRACE("child ending");
		return 0;
	}
	// lets do it again to see what happens if the child terminates abruptly...
	TRACE("forking a brat...");
	child_pid=fork();
	if(child_pid) {
		// parent
		int status;
		pid_t ret=waitpid(child_pid, &status, 0);
	} else {
		// child
		TRACE("child starting");
		TRACE("child ending");
		_exit(EXIT_SUCCESS);
	}
	// lets do some multi-threading work to see how multi-threading is affecting all this...
	const int num=2;
	pthread_t threads[num];
	int ids[num];
	void* rets[num];

	TRACE("before pthread_create");
	for(int i=0; i<num; i++) {
		ids[i]=i;
		CHECK_ZERO_ERRNO(pthread_create(threads+i, NULL, worker, ids+i));
	}
	TRACE("created threads");
	for (int i=0; i < num; i++) {
		CHECK_ZERO_ERRNO(pthread_join(threads[i], rets + i));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
