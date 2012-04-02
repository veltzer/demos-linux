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
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>

#include<us_helper.h>

/*
 * This example explores how to use thread priorities
 *
 * EXTRA_LIBS=-lpthread
 */

pthread_t hpt;
pthread_t mpt;
pthread_t lpt;

int lp = 2;
int mp = 1;
int hp = 0;

pthread_barrier_t mybarrier;

int min_priority = 0;




void *thread_body(void *arg) {
	pid_t tid = gettid();
	int pri = *(int *)arg;
	int err = nice(pri);

	if (err == -1) {
		printf("got error from nice(2)\n");
		exit(-1);
	}
	printf("thread %d starting\n", tid);
	printf("pri is %d\n", pri);
	while(true) {
		//pthread_barrier_wait(&mybarrier);
		print_scheduling_info();
		sleep(10);
	}
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	struct sched_param my_param;
	pthread_attr_t hp_attr;
	pthread_attr_t mp_attr;
	pthread_attr_t lp_attr;

	/* MAIN-THREAD WITH LOW PRIORITY */
	my_param.sched_priority = sched_get_priority_min(SCHED_FIFO);
	pthread_setschedparam(pthread_self(), SCHED_RR, &my_param);
	print_scheduling_info();
	/* SCHEDULING POLICY AND PRIORITY FOR OTHER THREADS */
	pthread_attr_init(&lp_attr);
	pthread_attr_init(&mp_attr);
	pthread_attr_init(&hp_attr);
	pthread_attr_setinheritsched(&lp_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&mp_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&hp_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&lp_attr, SCHED_FIFO);
	pthread_attr_setschedpolicy(&mp_attr, SCHED_FIFO);
	pthread_attr_setschedpolicy(&hp_attr, SCHED_FIFO);
	my_param.sched_priority = min_priority + 1;
	pthread_attr_setschedparam(&lp_attr, &my_param);
	my_param.sched_priority = min_priority + 2;
	pthread_attr_setschedparam(&mp_attr, &my_param);
	my_param.sched_priority = min_priority + 3;
	pthread_attr_setschedparam(&hp_attr, &my_param);
	pthread_barrier_init(&mybarrier, NULL, 1);
	pthread_create(&lpt, NULL, thread_body, &lp);
	pthread_create(&mpt, NULL, thread_body, &mp);
	pthread_create(&hpt, NULL, thread_body, &hp);
	pthread_join(hpt, NULL);
	pthread_join(mpt, NULL);
	pthread_join(lpt, NULL);
	printf("main exiting\n");
	return EXIT_SUCCESS;
}
