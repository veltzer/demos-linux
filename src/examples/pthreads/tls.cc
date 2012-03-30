#include<pthread.h> // for pthread_key_t, pthread_key_create(3), pthread_setspecific(3), pthread_getspecific(3), pthread_create(3), pthread_join(3)

#include"us_helper.hh" // for CHECK_*, TRACE

/*
 * This is a demo of thread local storage using the pthread API.
 *
 * 		Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */

pthread_key_t key_myid;

void* worker(void* arg) {
	CHECK_NOT_M1(pthread_setspecific(key_myid,arg));
	// now lets pull our id
	int myid=*(int*)pthread_getspecific(key_myid);
	TRACE("my id is %d\n",myid);
	return NULL;
}

void id_dealloc(void* ptr) {
	TRACE("deleting %p",ptr);
	int* p=(int*)ptr;
	delete p;
}

int main(int argc, char** argv, char** envp) {
	TRACE("start");
	CHECK_NOT_M1(pthread_key_create(&key_myid,id_dealloc));
	const unsigned int num=4;
	pthread_t threads[num];
	for (unsigned int i = 0; i < num; i++) {
		int* p=new int(i);
		TRACE("allocated %p",p);
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, p));
	}
	TRACE("created threads, now joining...");
	for (unsigned int i = 0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i],NULL));
	}
	TRACE("end");
	return EXIT_SUCCESS;
}
