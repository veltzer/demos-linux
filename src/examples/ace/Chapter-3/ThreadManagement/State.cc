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
#include<ace/Task.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_CMDS=pkg-config --cflags --libs ACE
*/

class HA_CommandHandler:public ACE_Task_Base {
	public:
		virtual int svc(void) {
			ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%t) Handler Thread running\n")));
			return(0);
		}
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Main Thread running\n")));
	HA_CommandHandler handler;
	int result=handler.activate(THR_NEW_LWP | THR_JOINABLE | THR_SUSPENDED);
	ACE_ASSERT(result == 0);
	ACE_UNUSED_ARG(result);
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) The current thread count is %d\n"), handler.thr_count()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) The group identifier is %d\n"), handler.grp_id()));
	handler.resume();
	handler.wait();
	return EXIT_SUCCESS;
}
