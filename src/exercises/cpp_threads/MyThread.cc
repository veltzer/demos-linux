#include <pthread.h>
#include <iostream>

#include "MyThread.hh"
#include "us_helper.hh"

void* MyThread::realsvc(void* arg) {
	MyThread* t=(MyThread*)arg;
	t->svc();
	return NULL;
}

MyThread::MyThread() {
}

void MyThread::start() {
	CHECK_ZERO(pthread_create(&myid,NULL,realsvc,(void*)this));
}

void MyThread::join() {
	CHECK_ZERO(pthread_join(myid,NULL));
}
