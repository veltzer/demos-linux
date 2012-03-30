#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

/*
 * EXTRA_LIBS=-lpthread
 */

void* PrintHello(void *threadid)
{
	int *id_ptr, taskid;
	id_ptr = (int *) threadid;
	taskid = *id_ptr;
	printf("Thread %d says hello\n",taskid);
	return (void *)(taskid*2);
	// another option would be to call
	//pthread_exit((void *)taskid);
}
int main(int argc,char** argv,char** envp)
{
	const int NUM_THREADS=8;
	pthread_t threads[NUM_THREADS];
	int t[NUM_THREADS];
	// init all values to be passed to threads
	for(int i=0;i<NUM_THREADS;i++) {
		t[i]=i;
	}
	// start all threads giving each it's input
	for(int i=0;i<NUM_THREADS;i++)
	{
		int rc=pthread_create(&threads[i],NULL,PrintHello,
			(void *)&t[i]);
		if(rc) {
			perror("could not create thread");
			exit(errno);
		}
	}
	// wait for all threads to finish, order does not matter
	for(int i=0;i<NUM_THREADS;i++)
	{
		void* retval;
		int rc=pthread_join(threads[i],&retval);
		if(rc) {
			perror("could not join thread");
			exit(errno);
		}
		printf("thread %d returned value %d\n",i,(int)retval);
	}
	return 0;
}
