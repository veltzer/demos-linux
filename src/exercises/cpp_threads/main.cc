#include"MyThread.hh"
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
