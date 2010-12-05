#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)
#include<sys/types.h> // for getpid(2)
#include<unistd.h> // for getpid(2), sleep(3)
#include<string.h> // for strncpy(3)
#include<sys/prctl.h> // for prctl(2)

#include"us_helper.hh"

/*
 * This exapmles shows how to set thread names in Linux.
 * The heard of the idea is to call prctl(2).
 * If you look at the documentation of prctl(2), it states that it only sets PROCESS
 * names, but this is actually wrong since it sets the name of the current schedualable
 * entity which may be a thread or a process. The reason that it says what is says is
 * mostly historical.
 *
 * 			Mark Veltzer
 * EXTRA_LIBS=-lpthread
 *
 * TODO:
 * - don't do the ugly sleep(3) here but rather wait on a condition in the threads
 *   and have the main thread signal that condition after doing the "ps" command.
 */

void set_thread_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name,newname,size);
	scie(prctl(PR_SET_NAME,name),"prctl");
}

void print_thread_name_from_proc() {
	my_system("cat /proc/%d/comm",gettid());
}

void* doit(void* arg) {
	const char* name=(const char*)arg;
	set_thread_name(name);
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	print_thread_name_from_proc();
	sleep(10);
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	printf("gettid() is %d\n",gettid());
	printf("getpid() is %d\n",getpid());
	printf("pthread_self() is %u\n",(unsigned int)pthread_self());
	pthread_t t1,t2;
	CHECK_ZERO(pthread_create(&t1,NULL,doit,(void*)"thread one"));
	CHECK_ZERO(pthread_create(&t2,NULL,doit,(void*)"thread two"));
	// show the threads with their names...
	my_system("ps -p %d -L",getpid());
	CHECK_ZERO(pthread_join(t1,NULL));
	CHECK_ZERO(pthread_join(t2,NULL));
	return 0;
}
