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
#include<ace/Thread_Hook.h>
#include<ace/Task.h>
#include<ace/Log_Msg.h>
#include"SecurityContext.hh"
#include<stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_CMDS=pkg-config --cflags --libs ACE
*/

class HA_ThreadHook:public ACE_Thread_Hook {
	public:
		virtual ACE_THR_FUNC_RETURN start(ACE_THR_FUNC func, void *arg) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) New Thread Spawned\n")));
			// Create the context on the thread's own stack.
			ACE_TSS<SecurityContext> secCtx;
			// Special initialization.
			add_sec_context_thr(secCtx);
			return((*func)(arg));
		}
		void add_sec_context_thr(ACE_TSS<SecurityContext>& secCtx);
};

void HA_ThreadHook::add_sec_context_thr(ACE_TSS<SecurityContext>& secCtx) {
	secCtx->user=0;
}

class HA_CommandHandler:public ACE_Task_Base {
	public:
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up \n")));
			// Do something.
			return(0);
		}
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	HA_ThreadHook hook;
	ACE_Thread_Hook::thread_hook(&hook);
	HA_CommandHandler handler;
	handler.activate();
	handler.wait();
	return EXIT_SUCCESS;
}
