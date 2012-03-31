#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

/*
 * EXTRA_LIBS=-lpthread
 */

const int NUM_THREADS=8;
void *PrintHello(void *threadid)
{
	int *id_ptr, taskid;
	id_ptr = (int *) threadid;
	int stime;
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	taskid = *id_ptr;
	printf("Thread %d says hello\n", taskid);
	pthread_exit( (void *)taskid);
}
int main(int argc,char** argv,char** envp)
{
	pthread_t threads[NUM_THREADS];
	int t[NUM_THREADS];
	int retval[NUM_THREADS];
	int stime;

	srand(time(0));
	for(int i=0;i<NUM_THREADS;i++)
	{
		t[i] = i;
		int rc = pthread_create(&threads[i], NULL, PrintHello,
			(void *) & t[i]);
		if(rc) {
			perror("could not create thread");
			exit(errno);
		}
	}
	for(int i=0;i<NUM_THREADS;i++)
	{
		stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
		sleep(stime);
		void* cretval;
		int rc=pthread_join(threads[i],&cretval);
		if(rc) {
			perror("could not join thread");
			exit(errno);
		}
		retval[i]=(int)cretval;
		printf("thread returned value %d\n",retval[i]);
	}
	sleep(1);
	return 0;
}
