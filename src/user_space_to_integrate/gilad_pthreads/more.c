#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

void * my_thread(void * param) {
	long i = (long)param;
	
	printf("I'm thread number %ld\n", i);
	
	pthread_exit(NULL);
}

int main(void) {
	
	
	pthread_t t[NUM_THREADS];
	long i;
	void * status;
	
	printf("Hello threads!\n");
	
	for(i=0; i< NUM_THREADS; i++) {
		pthread_create(&t[i], NULL, my_thread, (void *)i);
	}
	
	for(i=0; i< NUM_THREADS; i++) {
			pthread_join(t[i], &status);
			printf("Thread %d joined!\n", i);
	}
	
	printf("Goodbye threads!\n");
	
	
}
