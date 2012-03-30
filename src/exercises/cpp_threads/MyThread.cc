#include <pthread.h> // for pthread_create(3), pthread_join(3)

#include "MyThread.hh"
#include "us_helper.hh" // for CHECK_ZERO
 
/*
 * EXTRA_LIBS=-lpthread
 */

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

#include<iostream> // for std::cout, std::endl
#include<unistd.h> // for sleep(3)

#include "us_helper.hh" // for gettid()

class ImpThread:public MyThread {
	private:
		int limit;
		int sleep_time;

	public:
		ImpThread(int ilimit,int isleep_time):limit(ilimit),sleep_time(isleep_time) {}
	protected:
		virtual void svc() {
			pid_t tid=gettid();
			std::cout << "thread " << tid << " starting" << std::endl;
			for(int i=0;i<limit;i++) {
				std::cout << "Hello from thread " << tid << " num is " << i << std::endl;
				sleep(sleep_time);
			}
			std::cout << "thread " << tid << " ending" << std::endl;
		}
};

int main(int argc,char** argv,char** envp) {
	ImpThread thr1(10,1);
	ImpThread thr2(5,2);
	thr1.start();
	thr2.start();
	thr1.join();
	thr2.join();
	return 0;
}
