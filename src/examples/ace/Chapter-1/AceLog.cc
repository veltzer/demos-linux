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
#define ACE_NTRACE 0
#include <ace/Log_Msg.h>// for ACE_TRACE(), ACE_DEBUG()
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a basic example of using ACE_DEBUG and ACE_TRACE.
 *
 * Notes:
 * - the ACE_NTRACE macro controls whether trace does anything special or
 * not.
 * - the "%I" ACE_DEBUG modifier adds indentation level which matches the trace
 * level.
 * - you have to put your own newline at the end of ACE_DEBUG messages.
 *
 * EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
 * EXTRA_LINK_CMDS=pkg-config --libs ACE
 */
void foo(void) {
	ACE_TRACE(ACE_TEXT("foo"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHowdy Pardner\n")));
}

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	ACE_TRACE(ACE_TEXT("main"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHi Mom\n")));
	foo();
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnight\n")));
	foo();
	return EXIT_SUCCESS;
}
