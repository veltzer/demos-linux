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
#include<pthread.h> // for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include<unistd.h> // for sleep(3)
#include<us_helper.h> // for CHECK_ZERO(), printf(), gettid()

/*
* This example explores the relations between threads and signal handling.
* If we have more than one thread which thread will be used to handle a signal?
*
* Notes:
* - you can signal threads from the command line using kill(1). You just have
* to know the thread id.
* - If you register a signal handler from the main thread before the creation of
* the child threads then they too will have the signal handler installed.
* - You can also do it after the child threads are created.
* - What if a signal is registered by the child? Same. All threads get the handler.
*
* EXTRA_LIBS=-lpthread
*/
// count the number of signals we get
static unsigned int counter=0;

static void handler(int sig, siginfo_t *si, void *unused) {
	printf("sighandler: gettid() is %d\n",gettid());
	printf("sighandler: counter is %d\n",counter);
	printf("sighandler: got signal %s\n",strsignal(sig));
	printf("sighandler: si is %p\n",si);
	printf("sighandler: address is: 0x%lx\n",(long) si->si_addr);
	printf("sighandler: psiginfo follows...\n");
	psiginfo(si,"sighandler");
	counter++;
}

void* worker(void* p) {
	int num=*(int *)p;
	printf("thread starting num=%d, gettid()=%d, pthread_self()=%lu\n", num, gettid(), pthread_self());
	if(num==0) {
		register_handler_sigaction(SIGUSR1,handler);
	}
	// this is the right way to sleep even while accepting signals.
	// signal arrival will break the sleep and sleep will return number
	// of seconds remaining to sleep.
	int to_sleep=60;
	while((to_sleep=sleep(to_sleep))) {
		printf("wakeup because of signal\n");
	}
	printf("thread ending num=%d, gettid()=%d, pthread_self()=%lu\n", num, gettid(), pthread_self());
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	const int num=2;
	pthread_t threads[num];
	int ids[num];
	void* rets[num];

	//register_handler_sigaction(SIGUSR1,handler);
	printf("my pid is %d\n",getpid());
	printf("signal me with [kill -s SIGUSR1 %d]\n",getpid());
	printf("signal me with [kill -s SIGUSR2 %d]\n",getpid());

	printf("main starting\n");
	printf("main started creating threads\n");
	for(int i=0;i<num;i++) {
		ids[i]=i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	printf("main ended creating threads\n");
	sleep(1);
	register_handler_sigaction(SIGUSR2,handler);
	printf("main started joining threads\n");
	for (int i=0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	printf("main ended joining threads\n");
	printf("main ended\n");
	return EXIT_SUCCESS;
}
