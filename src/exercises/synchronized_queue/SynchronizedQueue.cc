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
#include <stdio.h>	// for fprintf(3)
#include <pthread.h>	// for pthread_create(3), pthread_join(3), pthread_spin_init(3), pthread_spin_destroy(3), pthread_spin_lock(3), pthread_spin_unlock(3), pthread_attr_init(3), pthread_attr_setaffinity_np(3)
#include <unistd.h>	// for sysconf(3)
#include <sched.h>	// for CPU_ZERO(3), CPU_SET(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <list>	// for STL list
#include <us_helper.h>	// for CHECK_ZERO()

/*
 * This is a solution to the synchronized queue exercise.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

template <typename T> class SynchronizedQueue {
private:
	std::list<T> mylist;
	pthread_mutex_t mymutex;
	pthread_cond_t mycond;
	unsigned int waiters;

public:
	SynchronizedQueue() {
		CHECK_ZERO(pthread_mutex_init(&mymutex, NULL));
		CHECK_ZERO(pthread_cond_init(&mycond, NULL));
		waiters=0;
	}
	void put(const T& t) {
		CHECK_ZERO(pthread_mutex_lock(&mymutex));
		mylist.push_back(t);
		// only wake up one getter if there are any getters
		// We only wake up one because we only put on data
		// element on the queue. If another putter comes
		// along he will wake up another one...
		if(waiters>0) {
			CHECK_ZERO(pthread_cond_signal(&mycond));
		}
		CHECK_ZERO(pthread_mutex_unlock(&mymutex));
	}
	T get() {
		CHECK_ZERO(pthread_mutex_lock(&mymutex));
		waiters++;
		while (mylist.empty()) {
			CHECK_ZERO(pthread_cond_wait(&mycond, &mymutex));
		}
		// the list is not empty so we can take an element
		T t=mylist.front();
		mylist.pop_front();
		waiters--;
		CHECK_ZERO(pthread_mutex_unlock(&mymutex));
		return t;
	}
};

// testing code starts here

typedef struct _thread_data {
	unsigned int num;
	unsigned int count;
	unsigned int max_sleep;
	unsigned int max_msg;
	bool producer;
	SynchronizedQueue<unsigned int>* queue;
} thread_data;

void *worker(void *p) {
	thread_data* td=(thread_data *)p;
	if(td->producer) {
		for(unsigned int i=0; i<td->count; i++) {
			td->queue->put(rand()%td->max_msg);
			usleep(rand()%td->max_sleep);
		}
		td->queue->put(td->max_msg);
	} else {
		while(true) {
			unsigned int i=td->queue->get();
			// TRACE("I got [%d]",i);
			if(i==td->max_msg) {
				break;
			}
		}
	}
	return(NULL);
}

int main(int argc, char** argv, char** envp) {
	SynchronizedQueue<unsigned int> queue;
	const unsigned int count=10000;
	const unsigned int max_sleep=100;
	const unsigned int max_msg=100;
	const unsigned int cpu_num=sysconf(_SC_NPROCESSORS_ONLN);
	const unsigned int thread_num=cpu_num;
	pthread_t* threads=new pthread_t[thread_num];
	pthread_attr_t* attrs=new pthread_attr_t[thread_num];
	thread_data* tds=new thread_data[thread_num];
	cpu_set_t* cpu_sets=new cpu_set_t[thread_num];
	for(unsigned int i=0; i<thread_num; i++) {
		tds[i].num=i;
		tds[i].producer=(i%2==0);
		tds[i].count=count;
		tds[i].max_sleep=max_sleep;
		tds[i].max_msg=max_msg;
		tds[i].queue=&queue;
		CPU_ZERO(cpu_sets+i);
		CPU_SET(i%cpu_num, cpu_sets+i);
		CHECK_ZERO(pthread_attr_init(attrs+i));
		CHECK_ZERO(pthread_attr_setaffinity_np(attrs+i, sizeof(cpu_set_t), cpu_sets+i));
		CHECK_ZERO(pthread_create(threads+i, attrs+i, worker, tds+i));
	}
	for(unsigned int i=0; i<thread_num; i++) {
		CHECK_ZERO(pthread_join(threads[i], NULL));
	}
	delete threads;
	delete attrs;
	delete tds;
	delete cpu_sets;
	return EXIT_SUCCESS;
}
