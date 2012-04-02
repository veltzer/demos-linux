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

#ifndef __CLIENTCONTEXT_H_
#define __CLIENTCONTEXT_H_

#include<firstinclude.h>
#include<ace/Hash_Map_Manager.h>
#include<ace/Synch.h>

typedef ACE_Hash_Map_Manager<const char *, void *, ACE_Null_Mutex> Map;

// Client-specific context information.
class ClientContext {
	public:
		void *get_attribute (const char *name);
		void set_attribute (const char *name, void *value);
	private:
		Map attributeMap_;
};
#endif /* __CLIENTCONTEXT_H_ */
