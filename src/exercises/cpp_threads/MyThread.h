#ifndef __Thread_h
#define __Thread_h

#include <pthread.h>
#include <iostream>

const int max=100;

class MyThread {
	private:
		pthread_mutex_t** mutexes;
		pthread_t myid;
		int registered;
		void cleanup() {
			// release all mutexes that were acquired
			for(int i=0;i<registered;i++) {
				if(mutexes[i]!=NULL) {
					pthread_mutex_unlock(mutexes[i]);
				}
			}
		}
		static void* realsvc(void* arg) {
			MyThread* t=(MyThread*)arg;
			t->classsvc();
			return NULL;
		}
		void classsvc() {
			//std::cerr << "In classsvc" << std::endl;
			this->svc();
			cleanup();
		}
	public:
		MyThread() {
			mutexes=new pthread_mutex_t*[max];
			registered=0;
		}
		void start() {
			pthread_create(&myid,NULL,realsvc,(void*)this);
		}
		void wait() {
			void* ret;
			pthread_join(myid,&ret);
		}
		void acquire(pthread_mutex_t* m) {
			// really acquire the mutex via pthread
			pthread_mutex_lock(m);
			mutexes[registered++]=m;
		}
		void release(pthread_mutex_t* m) {
			pthread_mutex_unlock(m);
			for(int i=0;i<registered;i++) {
				if(mutexes[i]==m) {
					mutexes[i]=NULL;
				}
			}
		}
	protected:
		virtual void svc()=0;
};

#endif // __Thread_h
