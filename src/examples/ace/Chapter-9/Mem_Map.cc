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
#include <ace/OS_NS_fcntl.h>
#include <ace/OS_NS_fcntl.h>
#include <ace/OS_NS_string.h>
#include <ace/Mem_Map.h>
#include <ace/Log_Msg.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
* EXTRA_LINK_CMDS=pkg-config --libs ACE
*/

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_HANDLE srcHandle=ACE_OS::open(argv[1],O_RDONLY);
	ACE_ASSERT(srcHandle!=ACE_INVALID_HANDLE);
	ACE_Mem_Map srcMap(srcHandle,static_cast<size_t>(-1),PROT_READ,ACE_MAP_PRIVATE);
	ACE_ASSERT(srcMap.addr()!=0);
	ACE_Mem_Map destMap(argv[2],
		srcMap.size(),
		O_RDWR | O_CREAT,
		ACE_DEFAULT_FILE_PERMS,
		PROT_RDWR,
		ACE_MAP_SHARED
	);
	ACE_ASSERT(destMap.addr()!=0);
	ACE_OS::memcpy(destMap.addr(),srcMap.addr(),srcMap.size());
	destMap.sync();
	srcMap.close();
	destMap.close();
	return EXIT_SUCCESS;
}
