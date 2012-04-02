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

#ifndef __CLIENTSERVICE_H_
#define __CLIENTSERVICE_H_

#include<firstinclude.h>
#include<ace/Synch_Traits.h>
#include<ace/Null_Condition.h>
#include<ace/Null_Mutex.h>
#include<ace/Message_Block.h>
#include<ace/SOCK_Stream.h>
#include<ace/Svc_Handler.h>

class ClientService:public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	public:
		int open(void* =0);
		// Called when input is available from the client.
		virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
		// Called when output is possible.
		virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);
		// Called when this handler is removed from the ACE_Reactor.
		virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
};

#endif /* __CLIENTSERVICE_H_ */
