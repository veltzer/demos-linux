#include<pthread.h> // for pthread_create, pthread_join
#include<stdio.h> // for printf(3)
#include<sys/types.h> // for getpid(2)
#include<unistd.h> // for getpid(2)
#include<string.h> // for strncpy(3)
#include<sys/prctl.h> // for prctl(2)

#include"us_helper.hh"

/*
 * This exapmles shows how to set thread names in Linux.
 * The heart of the idea is to call prctl(2).
 * If you look at the documentation of prctl(2), it states that it only sets PROCESS
 * names, but this is actually wrong since it sets the name of the current schedualable
 * entity which may be a thread or a process. The reason that it says what is says is
 * mostly historical.
 *
 * 			Mark Veltzer
 * EXTRA_LIBS=-lpthread
 */

void get_thread_name(char* buffer,unsigned int bufsize) {
	const unsigned int size=16;
	char name[size];
	CHECK_NOT_M1(prctl(PR_GET_NAME,name));
	strncpy(buffer,name,bufsize);
}

void set_thread_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name,newname,size);
	CHECK_NOT_M1(prctl(PR_SET_NAME,name));
}

void print_thread_name_from_proc() {
	my_system("cat /proc/%d/comm",gettid());
}

typedef struct _thread_data {
	pthread_mutex_t start_mutex;
	pthread_mutex_t end_mutex;
	char name[256];
} thread_data;

void* doit(void* arg) {
	char orig_name[256];
	get_thread_name(orig_name,256);
	TRACE("original thread name is [%s]",orig_name);
	thread_data* td=(thread_data*)arg;

	set_thread_name(td->name);
	TRACE("gettid() is %d",gettid());
	TRACE("getpid() is %d",getpid());
	TRACE("pthread_self() is %u",(unsigned int)pthread_self());
	print_thread_name_from_proc();
	CHECK_ZERO(pthread_mutex_unlock(&(td->start_mutex)));
	CHECK_ZERO(pthread_mutex_lock(&(td->end_mutex)));
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	TRACE("gettid() is %d",gettid());
	TRACE("getpid() is %d",getpid());
	TRACE("pthread_self() is %u",(unsigned int)pthread_self());
	pthread_t t1,t2;
	thread_data td1,td2;
	strncpy(td1.name,"thread one",256);
	strncpy(td2.name,"thread two",256);
	CHECK_ZERO(pthread_mutex_init(&td1.start_mutex,NULL));
	CHECK_ZERO(pthread_mutex_init(&td2.start_mutex,NULL));
	CHECK_ZERO(pthread_mutex_init(&td1.end_mutex,NULL));
	CHECK_ZERO(pthread_mutex_init(&td2.end_mutex,NULL));
	CHECK_ZERO(pthread_mutex_lock(&td1.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td1.end_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.end_mutex));
	CHECK_ZERO(pthread_create(&t1,NULL,doit,&td1));
	CHECK_ZERO(pthread_create(&t2,NULL,doit,&td2));

	// wait for the threads to be initialized, if we got the lock then they are...
	CHECK_ZERO(pthread_mutex_lock(&td1.start_mutex));
	CHECK_ZERO(pthread_mutex_lock(&td2.start_mutex));

	// now that both threads have set their name, show the threads with their names...
	my_system("ps -p %d -L",getpid());
	// let the threads die (if we do not unlock they will wait forever...)
	CHECK_ZERO(pthread_mutex_unlock(&td1.end_mutex));
	CHECK_ZERO(pthread_mutex_unlock(&td2.end_mutex));
	// join the theads so that everything will be clean...
	CHECK_ZERO(pthread_join(t1,NULL));
	CHECK_ZERO(pthread_join(t2,NULL));
	return EXIT_SUCCESS;
}
