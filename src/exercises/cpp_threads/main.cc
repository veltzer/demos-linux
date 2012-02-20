#include"MyThread.h"
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/syscall.h>

/*
 * EXTRA_LIBS=-lpthread
 */

class ImpThread:public MyThread {
	private:
		int limit;
		int sleep_time;

	public:
		ImpThread(int ilimit,int isleep_time):limit(ilimit),sleep_time(isleep_time) {}
	protected:
		virtual void svc() {
			pid_t tid=syscall(SYS_gettid);
			std::cerr << "thread " << tid << " starting" << std::endl;
			for(int i=0;i<limit;i++) {
				std::cerr << "Hello from thread " << tid << " num is " << i << std::endl;
				sleep(sleep_time);
			}
			std::cerr << "thread " << tid << " ending" << std::endl;
		}
};

int main(int argc,char** argv,char** envp) {
	ImpThread thr1(10,1);
	ImpThread thr2(5,2);
	thr1.start();
	thr2.start();
	thr1.wait();
	thr2.wait();
	return 0;
}
