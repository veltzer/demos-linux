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
#include <ace/SOCK_Acceptor.h>
#include <ace/Log_Msg.h>
#include <ace/Read_Buffer.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_CMDS=pkg-config --cflags --libs ACE
*/

// An ACE_Read_Buffer pointing to STDIN
static ACE_Read_Buffer rb(ACE_STDIN);

int GetMessageType(char* data) {
	// read a single line from stdin
	// Allocate a new buffer.
	char* buffer=rb.read('\n');

	if(buffer==NULL) {
		// return message type zero when EOF is reached
		return(0);
	} else {
		int type;
		sscanf(buffer,"%d",&type);
		// Remove the type from the buffer
		ACE_OS::sprintf(data,"%s",buffer+2);
		return(type);
	}
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_INET_Addr port_to_listen(20000);
	ACE_SOCK_Acceptor acceptor;
	int type=1;
	char buffer[4096];
	if(acceptor.open(port_to_listen,1)==-1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("acceptor.open")), 100);
	}
	ACE_SOCK_Stream peer;

	/*
	* Basic acceptor usage - No timeout
	*/
// #define NO_TIMEOUT
#ifdef NO_TIMEOUT
	if(acceptor.accept(peer)==-1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) Failed to accept client connection\n")), 100);
	}
#else
	ACE_Time_Value timeout(10, 0);
	ACE_INET_Addr peer_addr;
	if (acceptor.accept(peer, &peer_addr, &timeout, 0)==-1) {
		if (ACE_OS::last_error()==EINTR) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Interrupted while waiting for connection\n")));
		} else if (ACE_OS::last_error()==ETIMEDOUT) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Timeout while waiting for connection\n")));
		}
	} else {
		ACE_TCHAR peer_name[MAXHOSTNAMELEN];
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Connection from %s\n"), peer_name));
	}
#endif /* NO_TIMEOUT */
	while(type!=0) {
		type=GetMessageType(buffer);
		// if type is not 0 (EOF) then write it to the client
		if (type!=0) {
			size_t size=ACE_OS::strlen(buffer);
			buffer[size++]='\n';
			peer.send_n(buffer, size, 0);
			peer.recv(buffer, sizeof(buffer));
		}
	}
	peer.close();
	return EXIT_SUCCESS;
}
