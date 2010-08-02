#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for exit(3)
#include<unistd.h> // for syscall(2)
#include<sys/types.h> // for gettid(2) 
#include<sys/syscall.h> // for syscall(2)

/*
	This demo explored gettid() and getpid() issues on linux
	This demo shows that:
	- for the main thread (the one going from the main function) the thread id
	is the same as the process id.
	- for other threads the process id is the same but the thread id is different.
	- pthread_self() is different than all of the above (some opaque value).
	- there is no wrapper function in libc for this one. Just create the wrapper
	yourself or use some framework which gives you wrappers.
	- there are no "thread names" in linux. There are process names (although even
	they are not unique in any way) but no thread names. The way to get thread names
	is to create some user space construct that stores that information.
 
EXTRA_LIBS=-lpthread
*/

pid_t gettid(void) {
	return syscall(SYS_gettid);
}

void* doit(void*) {
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	pthread_t t1,t2;
	int res;
	res=pthread_create(&t1,NULL,doit,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
	res=pthread_create(&t2,NULL,doit,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_create");
		exit(-1);
	}
	res=pthread_join(t1,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_join");
		exit(-1);
	}
	res=pthread_join(t2,NULL);
	if(res!=0) {
		fprintf(stderr,"error in pthread_join");
		exit(-1);
	}
	return 0;
}
