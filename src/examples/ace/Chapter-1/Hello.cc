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
#include <ace/Log_Msg.h> // for ACE_DEBUG and ACE_ERROR
#include <ace/OS_NS_stdio.h> // for the printf function
#include <ace/streams.h> // for C++ streams interface
#include <stdlib.h> // for EXIT_SUCCESS

/*
* Short program demonstrates printing Hello World using the ACE environment
* notice that both ACE_DEBUG and ACE_ERROR are going to stderr.
*
* EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
* EXTRA_LINK_CMDS=pkg-config --libs ACE
*/

// Notice the use of the ACE version of main
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	printf("It seems that printf is working\n");
	cout << "It seems like C++ streams are working without std:: prefix" << endl;
	// Notice that ACE_DEBUG takes one argument
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("ACE_DEBUG(pid=%P Time:%T)\n")));
	ACE_ERROR((LM_ERROR, ACE_TEXT("ACE_ERROR(pid=%P File:%N Line:%l Date+Time:%D)\n")));
	return EXIT_SUCCESS;
}
