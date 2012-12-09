/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for CHECK_ZERO()
#include "mythread.hh"

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

#include <iostream> // for std::cout, std::endl
#include <unistd.h> // for sleep(3)
#include <us_helper.h> // for gettid()

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
	return EXIT_SUCCESS;
}
