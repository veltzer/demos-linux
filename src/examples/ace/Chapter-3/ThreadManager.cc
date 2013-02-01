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

#include <firstinclude.h>
#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <ace/Thread_Manager.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
* EXTRA_LINK_CMDS=pkg-config --libs ACE
*/

class ExitHandler:public ACE_At_Thread_Exit {
	private:
		int a;
	protected:
		virtual void apply() {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) doing exit\n")));
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) a is %d\n"),a));
			//delete a;
		}
	public:
		void setA(int ia) {
			a=ia;
		}
};

class HA_CommandHandler:public ACE_Task_Base {
	private:
		ExitHandler& eh_;
		int val;
	public:
		HA_CommandHandler(ExitHandler & eh, int ival):eh_(eh),val(ival) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) at HA_CommandHandler\n")));
		}
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up\n")));
			thr_mgr()->at_exit(eh_);
			//int *a=new int(val);
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

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
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
	return EXIT_SUCCESS;
}
