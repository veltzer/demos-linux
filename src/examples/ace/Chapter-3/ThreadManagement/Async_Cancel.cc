/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<firstinclude.h>
#include<ace/OS_NS_unistd.h>
#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_CMDS=pkg-config --cflags --libs ACE
*/

class CanceledTask:public ACE_Task<ACE_MT_SYNCH> {
public:
	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Starting thread\n")));

		if (this->set_cancel_mode() < 0) {
			return(-1);
		}

		while(true) {
			// Put this thread in a compute loop.. no
			// cancellation points are available.
		}
#if defined (__HP_aCC)
		// This is only to workaround a warning on HP-UX compiler.
		return(0);
#endif /* __HP_aCC */
	}
	int set_cancel_mode(void) {
		cancel_state new_state;

		// Set the cancel state to asynchronous and enabled.
		new_state.cancelstate=PTHREAD_CANCEL_ENABLE;
		new_state.canceltype=PTHREAD_CANCEL_ASYNCHRONOUS;
		if (ACE_Thread::setcancelstate(new_state, 0) == -1) {
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("cancelstate")), -1);
		}
		return(0);
	}
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	CanceledTask task;
	task.activate();
	ACE_OS::sleep(1);
	ACE_Thread_Manager::instance()->cancel_task(&task, 1);
	task.wait();
	return EXIT_SUCCESS;
}
