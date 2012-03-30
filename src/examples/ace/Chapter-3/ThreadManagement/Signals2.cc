#include<ace/config-lite.h>
#include<ace/OS_NS_time.h>
#include<ace/OS_NS_unistd.h>
#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include<ace/Signal.h>
#include<ace/Sig_Handler.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class SignalableTask : public ACE_Task<ACE_MT_SYNCH> {
public:
	virtual int handle_signal(int signum,
		siginfo_t * = 0,
		ucontext_t * = 0) {
		if (signum == SIGUSR1) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) received a %S signal\n"), signum));
			handle_alert();
		}

		return(0);
	}


	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Starting thread\n")));

		while (1) {
			ACE_Message_Block *mb = 0;
			ACE_Time_Value tv(0, 1000);

			tv += ACE_OS::time(0);

			int result = this->getq(mb, &tv);

			if ((result == -1) && (errno == EWOULDBLOCK)) {
				continue;
			} else {
				process_message(mb);
			}
		}

		ACE_NOTREACHED(return 0);
	}


	void handle_alert();
	void process_message(ACE_Message_Block *mb);
};

void
SignalableTask::process_message(ACE_Message_Block *) {
}


void
SignalableTask::handle_alert(void) {
}


// Listing 1 code/ch13
int ACE_TMAIN(int, ACE_TCHAR *[]) {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Main thread \n")));
	SignalableTask handler;
	handler.activate(THR_NEW_LWP | THR_JOINABLE, 5);

	ACE_Sig_Handler sh;
	sh.register_handler(SIGUSR1, &handler);

	// Allow threads to start
	ACE_OS::sleep(1);

	for (int i = 0; i < 5; i++) {
		ACE_OS::kill(ACE_OS::getpid(), SIGUSR1);
	}
	handler.wait();
	return(0);
}
