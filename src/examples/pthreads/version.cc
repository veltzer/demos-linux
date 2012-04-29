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
#include<stdio.h> // for printf(3)
#include<us_helper.h> // for CHECK_NOT_ZERO()
#include<unistd.h> // for confstr(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
* This demp shows how to get the version of the pthread library you are using.
*
* EXTRA_LIBS=-lpthread
*/

pthread_key_t key_myid;

int main(int argc,char** argv,char** envp) {
	const unsigned int bufsize=256;
	char name[bufsize];
	CHECK_IN_RANGE(confstr(_CS_GNU_LIBPTHREAD_VERSION,name,bufsize),1,bufsize);
	printf("confstr(_CS_GNU_LIBPTHREAD_VERSION) returns [%s]\n",name);
	return EXIT_SUCCESS;
}
