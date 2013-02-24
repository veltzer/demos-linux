/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
 * EXTRA_LINK_CMDS=pkg-config --libs ACE
 */

class ExitHandler : public ACE_At_Thread_Exit {
public:
	virtual void apply(void) {
		ACE_DEBUG((LM_INFO, ACE_TEXT("(%t) is exiting \n")));
		// Shut down all devices.
	}
};

class HA_CommandHandler : public ACE_Task_Base {
public:
	HA_CommandHandler(ExitHandler & eh) : eh_(eh) {
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

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	ExitHandler eh;
	HA_CommandHandler handler(eh);
	handler.activate();
	ACE_Thread_Manager::instance()->wait();
	return EXIT_SUCCESS;
}
/*
 * int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
 * ExitHandler eh;
 * ACE_Thread_Manager tm;
 * HA_CommandHandler handler(eh);
 * handler.thr_mgr(&tm);
 * handler.activate();
 * tm.wait();
 * return EXIT_SUCCESS;
 * }
 */
