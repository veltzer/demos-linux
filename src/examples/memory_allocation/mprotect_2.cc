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
#include<stdio.h> // for perror(3)
#include<stdlib.h> // for exit(3), posix_memalign(3), EXIT_FAILURE, EXIT_SUCCESS
#include<unistd.h> // for getpagesize(2)
#include<sys/mman.h> // for mprotect(2)

// this code simulates a memory corruption
void bad_code(void* precious_data) {
	char* p=(char*)precious_data;
	p[0]=5;
}

void protect_me(void* precious_data,size_t len) {
	if(mprotect(precious_data,len,PROT_READ)==-1) {
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char** argv,char** envp) {
	void* precious_data;
	const unsigned int size=8192;
	if(posix_memalign(&precious_data,getpagesize(),size)==-1) {
		perror("posix_memalign");
		exit(EXIT_FAILURE);
	}
	protect_me(precious_data,size);
	bad_code(precious_data);
	return EXIT_SUCCESS;
}
