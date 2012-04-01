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

#include<ace/INET_Addr.h>
#include<ace/SOCK_Stream.h>
#include<ace/SOCK_Connector.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_INET_Addr srvr(50000, ACE_LOCALHOST);

	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;

	if (connector.connect(peer, srvr) == -1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("connect")), 1);
	}
	char buf[64];
	peer.send_n("uptime\n", 7);
	int bc = peer.recv(buf, sizeof(buf));
	int res = write(1, buf, bc);
	res++;
	peer.close();
	return EXIT_SUCCESS;
}
