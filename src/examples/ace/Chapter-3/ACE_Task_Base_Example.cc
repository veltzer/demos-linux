#include<ace/Task.h> // for ACE_Task_Base
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is a simple ACE_Task_Base showing how to create your own threads
 * in an OO fashion.
 *
 * Coding the thread:
 * - inherit from ACE_Task_Base
 * - create constructor for neccessary initialisation values.
 * - create methods that return the final return value.
 * - code the svc method. (what about it's return value?!?).
 *
 * Running the thread:
 * - create an instance of your class (supply neccessary arguments to constructor).
 * - .activate() to run the thread (returns error code).
 * - .wait() to wait for it to be over (returns error code).
 * - get your return values
 *
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class MyThread:public ACE_Task_Base {
	private:
		int val;
	public:
		MyThread(int ival):val(ival) {
		}
		int getVal() {
			return val;
		}
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) thread: starting\n")));
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) thread: ending\n")));
			return 0;
		}
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) main: starting\n")));
	MyThread thread(7);
	thread.activate();
	thread.wait();
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) main: res is %d\n"),thread.getVal()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) main: ending\n")));
	return EXIT_SUCCESS;
}
