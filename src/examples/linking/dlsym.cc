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
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for atoi(3), EXIT_SUCCESS
#include <dlfcn.h> // for dlsym(3)

/*
* This example shows how to override a symbol like atoi. We define our own atoi
* but also call the original implementation.
*
* EXTRA_LINK_FLAGS=-ldl
*/

int (*p_atoi)(const char* nptr);

void setup() __attribute__((constructor));
void setup() {
	p_atoi=(typeof(p_atoi))dlsym(RTLD_NEXT,"atoi");
}

int atoi(const char* nptr) {
	return p_atoi(nptr)/2;
}

int main(int argc,char** argv,char** envp) {
	printf("atoi of 45 is %d\n",atoi("45"));
	return EXIT_SUCCESS;
}
