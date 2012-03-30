#include<ace/config-lite.h>
#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include<ace/OS_NS_unistd.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH> {
public:
	HA_CommandHandler(const char *name) : name_(name) {
	}

	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up %C\n"), name_));

		ACE_OS::sleep(2);
		ACE_Message_Block *mb = 0;
		while (this->getq(mb) != -1) {
			if (mb->msg_type() == ACE_Message_Block::MB_BREAK) {
				mb->release();
				break;
			}
			process_message(mb);
			mb->release();
		}
		return(0);
	}


	void process_message(ACE_Message_Block *) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Processing message %C\n"), name_));
		// Simulate compute bound task.
		for (int i = 0; i < 100; i++) {
			;
		}
	}


private:
	const char *name_;
};

#if !defined (ACE_THR_PRI_OTHER_MAX)
// This should be fixed in ACE... There's no _MAX, _MIN values for thread priorities.
#if defined (ACE_WIN32)
#define ACE_THR_PRI_OTHER_MAX ((ACE_THR_PRI_OTHER_DEF)+1)
#elif defined (VXWORKS)
#define ACE_THR_PRI_OTHER_MAX 0
#endif
#endif

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	HA_CommandHandler hp_handler("HighPriority");

	hp_handler.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1, ACE_THR_PRI_OTHER_MAX);

	HA_CommandHandler lp_handler("LowPriority");

	lp_handler.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1, ACE_THR_PRI_OTHER_DEF);

	ACE_Message_Block mb;
	for (int i = 0; i < 100; i++) {
		ACE_Message_Block *mb_hp, *mb_lp;
		mb_hp = mb.clone();
		mb_lp = mb.clone();
		hp_handler.putq(mb_hp);
		lp_handler.putq(mb_lp);
	}

	ACE_Message_Block stop(0, ACE_Message_Block::MB_BREAK);

	hp_handler.putq(stop.clone());
	lp_handler.putq(stop.clone());
	hp_handler.wait();
	lp_handler.wait();

	return(0);
}
