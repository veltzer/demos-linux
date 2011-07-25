#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <ace/Thread_Manager.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class ExitHandler : public ACE_At_Thread_Exit {
private:
	int a;
protected:
	virtual void apply() {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) doing exit\n")));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) a is %d\n"), a));
		//delete a;
	}


public:
	void setA(int ia) {
		a = ia;
	}
};

class HA_CommandHandler : public ACE_Task_Base {
private:
	ExitHandler& eh_;
	int val;
public:
	HA_CommandHandler(ExitHandler & eh, int ival) : eh_(eh),val(ival) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) at HA_CommandHandler\n")));
	}


	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up\n")));
		thr_mgr()->at_exit(eh_);
		//int *a = new int(val);
		eh_.setA(val);
		// Do something.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) simulating some work\n")));
		sleep(3);
		// Forcefully exit.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) finishing thread\n")));
		// notice that we must call ACE_Thread::exit
		ACE_Thread::exit();
		// NOT REACHED
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) this will not be printed\n")));
		return(0);
	}
};

int ACE_TMAIN(int, ACE_TCHAR **) {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: starting up\n")));
	// first thread
	ExitHandler eh;
	HA_CommandHandler handler(eh,8);
	handler.activate();

	// second thread
	ExitHandler eh2;
	HA_CommandHandler handler2(eh2,9);
	handler2.activate();

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: waiting for both threads to finish\n")));
	// wait for both threads to finish...
	ACE_Thread_Manager::instance()->wait();
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: that's it folks...\n")));
	return(0);
}
