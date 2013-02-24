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
#include <ace/OS_NS_sys_time.h>
#include <ace/Log_Msg.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Reactor.h>
#include <ace/Message_Queue.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
* EXTRA_LINK_CMDS=pkg-config --libs ACE
*/

class ClientService:public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
	public:
		typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;
		virtual int open(void* =NULL);
};

int ClientService::open(void* p) {
	super::open(p);
	ACE_OS::printf("connected\n");
	return(0);
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	typedef ACE_Acceptor<ClientService, ACE_SOCK_ACCEPTOR> ClientAcceptor;
	//ACE_INET_Addr port_to_listen (50000, ACE_LOCALHOST );
	ACE_INET_Addr port_to_listen(50000);
	ClientAcceptor acceptor;
	acceptor.open(port_to_listen);
	ACE_Reactor::instance()->run_reactor_event_loop();
	return EXIT_SUCCESS;
}
