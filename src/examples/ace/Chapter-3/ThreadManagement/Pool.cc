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
#include <ace/config-lite.h>
#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
 * EXTRA_LINK_CMDS=pkg-config --libs ACE
 */

class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH> {
public:
	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) starting up\n")));
		ACE_Message_Block* mb;
		if(this->getq(mb)==-1) {
			return(-1);
		}
		// ... do something with the message.
		return(0);
	}
};

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	HA_CommandHandler handler;
	// Create 4 threads.
	handler.activate(THR_NEW_LWP | THR_JOINABLE, 4);
	handler.wait();
	return EXIT_SUCCESS;
}
