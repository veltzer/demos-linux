#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <ace/Thread_Manager.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class ExitHandler : public ACE_At_Thread_Exit {
private:
	int *a;
protected:
	virtual void apply() {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) doing exit\n")));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) a is %d\n"), *a));
		delete a;
	}


public:
	void setA(int *ia) {
		a = ia;
	}
};

class HA_CommandHandler : public ACE_Task_Base {
private:
	ExitHandler& eh_;
public:
	HA_CommandHandler(ExitHandler & eh) : eh_(eh) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) at HA_CommandHandler\n")));
	}


	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up\n")));
		thr_mgr()->at_exit(eh_);
		int *a = new int(8);
		eh_.setA(a);
		// Do something.
		// Forcefully exit.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) finishing thread\n")));
		// notice that we must call ACE_Thread::exit
		ACE_Thread::exit();
		// NOT REACHED
		return(0);
	}
};

int ACE_TMAIN(int, ACE_TCHAR **) {
	// first thread
	ExitHandler eh;

	HA_CommandHandler handler(eh);

	handler.activate();

	// second thread
	ExitHandler eh2;
	HA_CommandHandler handler2(eh2);

	handler2.activate();

	ACE_Thread_Manager::instance()->wait();
	return(0);
}
