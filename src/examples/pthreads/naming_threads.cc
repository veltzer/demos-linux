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
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <stdio.h>	// for printf(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <string.h>	// for strncpy(3)
#include <sys/prctl.h>	// for prctl(2)
#include <us_helper.h>	// for CHECK_ZERO()

/*
 * This exapmle shows how to set thread names in Linux.
 * The heart of the idea is to call prctl(2).
 * If you look at the documentation of prctl(2), it states that it only sets PROCESS
 * names, but this is actually wrong since it sets the name of the current schedualable
 * entity which may be a thread or a process. The reason that it says what is says is
 * mostly historical.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

void get_thread_name(char* buffer, unsigned int bufsize) {
	const unsigned int size=16;
	char name[size];
	CHECK_NOT_M1(prctl(PR_GET_NAME, name));
	strncpy(buffer, name, bufsize);
}

void set_thread_name(const char* newname) {
	// the 16 here is a limitation of prctl(2)...
	const unsigned int size=16;
	char name[size];
	strncpy(name, newname, size);
	CHECK_NOT_M1(prctl(PR_SET_NAME, name));
}

void print_thread_name_from_proc() {
	my_system("cat /proc/%d/comm", gettid());
}

typedef struct _thread_data {
	pthread_mutex_t start_mutex;
	pthread_mutex_t end_mutex;
	char name[256];
} thread_data;

void* doit(void* arg) {
	char orig_name[256];
	get_thread_name(orig_name, 256);
	TRACE("original thread name is [%s]", orig_name);
	thread_data* td=(thread_data*)arg;

	set_thread_name(td->name);
	TRACE("gettid() is %d", gettid());
	TRACE("getpid() is %d", getpid());
	TRACE("pthread_self() is %u", (unsigned int)pthread_self());
	print_thread_name_from_proc();
	CHECK_ZERO(pthread_mutex_unlock(&(td->start_mutex)));
	CHECK_ZERO(pthread_mutex_lock(&(td->end_mutex)));
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	TRACE("gettid() is %d", gettid());
	TRACE("getpid() is %d", getpid());
	TRACE("pthread_self() is %lu", pthread_self());
	pthread_t t1, t2;
	thread_data td1, td2;
	strncpy(td1.name, "thread one", 256);
	//strncpy(td2.name, "thread two", 256);
	strncpy(td2.name, "תהליכון", 256);
	CHECK_ZERO(pthread_mutex_init(&td1.start_mutex, NULL));
	CHECK_ZERO(pthread_mutex_init(&td2.start_mutex, NULL));
	CHECK_ZERO(pthread_mutex_init(&td1.end_mutex, NULL));
	CHECK_ZERO(pthread_mutex_init(&td2.end_mutex, NULL));
	CHECK_ZERO(pthread_mutex_lock(&td1.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td1.end_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.end_mutex));
	CHECK_ZERO(pthread_create(&t1, NULL, doit, &td1));
	CHECK_ZERO(pthread_create(&t2, NULL, doit, &td2));

	// wait for the threads to be initialized, if we got the lock then they are...
	CHECK_ZERO(pthread_mutex_lock(&td1.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.start_mutex));

	// now that both threads have set their name, show the threads with their names...
	my_system("ps -p %d -L", getpid());
	// let the threads die (if we do not unlock they will wait forever...)
	CHECK_ZERO(pthread_mutex_unlock(&td1.end_mutex));
	CHECK_ZERO(pthread_mutex_unlock(&td2.end_mutex));
	// join the theads so that everything will be clean...
	CHECK_ZERO(pthread_join(t1, NULL));
	CHECK_ZERO(pthread_join(t2, NULL));
	return EXIT_SUCCESS;
}
