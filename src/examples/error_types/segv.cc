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
#include<us_helper.h> // for TRACE()

/*
* This is a simple example causing a segmentation fault to be generated.
*
* The address "0" is not mapped into any regular processes virtual address space
* which is a good thing since NULL (which is heavily used in C programs) has the
* value 0.
*
* TODO:
* - show that you can catch SIGSEGV and even continue running.
*/

int main(int argc,char** argv,char** envp) {
	// lets show that the NULL constant is actually the number 0...
	TRACE("NULL is %d",NULL);
	char *p = 0;
	p[0] = 0;
	return EXIT_SUCCESS;
}
