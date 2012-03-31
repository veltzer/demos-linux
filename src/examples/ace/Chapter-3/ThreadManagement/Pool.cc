#include<ace/config-lite.h>
#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Mark Veltzer
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class HA_CommandHandler:public ACE_Task<ACE_MT_SYNCH> {
	public:
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) starting up\n")));
			ACE_Message_Block* mb;
			if(this->getq(mb)==-1) {
				return(-1);
			}
			// ... do something with the message.
			return(0);
		}
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	HA_CommandHandler handler;
	// Create 4 threads.
	handler.activate(THR_NEW_LWP | THR_JOINABLE, 4);
	handler.wait();
	return EXIT_SUCCESS;
}
