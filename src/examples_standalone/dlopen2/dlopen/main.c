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
#include<stdio.h> // for fprintf(3), printf(3)
#include<dlfcn.h> // for dlopen(3), dlsym(3), dlclose(3)
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	void* h;
	CHECK_NOT_NULL(h=dlopen("libadd.so", RTLD_NOW));
	void* sym;
	CHECK_NOT_NULL(sym=dlsym(h,"func"));
	int (*f)(int, int);
	f=((int(*)(int, int))sym);
	int result=f(2,2);
	printf("2+2 is %d\n",result);
	CHECK_ZERO(dlclose(h));
	return EXIT_SUCCESS;
}
