#ifndef __Thread_h
#define __Thread_h

#include<pthread.h>

class MyThread {
	private:
		pthread_t myid;
		static void* realsvc(void* arg);
	public:
		MyThread();
		void start();
		void join();
	protected:
		virtual void svc()=0;
};

#endif // __Thread_h
