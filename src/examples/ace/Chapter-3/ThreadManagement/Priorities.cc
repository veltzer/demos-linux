/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <ace/config-lite.h>
#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <ace/OS_NS_unistd.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
 * EXTRA_LINK_CMDS=pkg-config --libs ACE
 */

class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH> {
public:
	HA_CommandHandler(const char *name) : name_(name) {
	}
	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up %C\n"), name_));

		ACE_OS::sleep(2);
		ACE_Message_Block* mb=0;
		while(this->getq(mb)!=-1) {
			if(mb->msg_type()==ACE_Message_Block::MB_BREAK) {
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
		for(int i=0; i<100; i++) {
			sleep(1);
		}
	}

private:
	const char* name_;
};

#if !defined (ACE_THR_PRI_OTHER_MAX)
// This should be fixed in ACE... There's no _MAX, _MIN values for thread priorities.
#if defined (ACE_WIN32)
#define ACE_THR_PRI_OTHER_MAX ((ACE_THR_PRI_OTHER_DEF)+1)
#elif defined (VXWORKS)
#define ACE_THR_PRI_OTHER_MAX 0
#endif
#endif

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	HA_CommandHandler hp_handler("HighPriority");
	hp_handler.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1, ACE_THR_PRI_OTHER_MAX);
	HA_CommandHandler lp_handler("LowPriority");
	lp_handler.activate(THR_NEW_LWP | THR_JOINABLE, 1, 1, ACE_THR_PRI_OTHER_DEF);
	ACE_Message_Block mb;
	for(int i=0; i<100; i++) {
		ACE_Message_Block *mb_hp, *mb_lp;
		mb_hp=mb.clone();
		mb_lp=mb.clone();
		hp_handler.putq(mb_hp);
		lp_handler.putq(mb_lp);
	}
	ACE_Message_Block stop(0, ACE_Message_Block::MB_BREAK);
	hp_handler.putq(stop.clone());
	lp_handler.putq(stop.clone());
	hp_handler.wait();
	lp_handler.wait();
	return EXIT_SUCCESS;
}
