#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Mark Veltzer
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class ExitHandler:public ACE_At_Thread_Exit {
	public:
		virtual void apply(void) {
			ACE_DEBUG((LM_INFO, ACE_TEXT("(%t) is exiting \n")));
			// Shut down all devices.
		}
};

class HA_CommandHandler:public ACE_Task_Base {
	public:
		HA_CommandHandler(ExitHandler & eh):eh_(eh) {
		}
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up \n")));
			this->thr_mgr()->at_exit(eh_);
			// Do something.
			// Forcefully exit.
			ACE_Thread::exit();
			// NOT REACHED
			return(0);
		}
	private:
		ExitHandler& eh_;
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ExitHandler eh;
	HA_CommandHandler handler(eh);
	handler.activate();
	ACE_Thread_Manager::instance()->wait();
	return EXIT_SUCCESS;
}
/*
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ExitHandler eh;
	ACE_Thread_Manager tm;
	HA_CommandHandler handler(eh);
	handler.thr_mgr(&tm);
	handler.activate();
	tm.wait();
	return EXIT_SUCCESS;
}
*/
