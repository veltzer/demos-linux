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

#include<dlfcn.h> // for dlopen(3), dlclose(3), dlerror(3)
#include<stdlib.h> // for exit(3)
#include<stdio.h> // for fprintf(3)

#include"us_helper.hh"

/*
 * Static handle to the library and to the function
 */
static void* handle=NULL;
static double (*psin)(double);

/* Initialization function for this library
 * This function will load the real library using dlopen et al...
 */
static void init(void) {
	TRACE("in here");
	//handle=dlopen("/lib/tls/i686/cmov/libm.so.6", RTLD_LAZY);
	//handle=dlopen("/lib/libm-2.12.1.so", RTLD_LAZY);
	handle=dlopen(NULL, RTLD_LAZY);
	if(handle==NULL) {
		fprintf(stderr,"error in dlopen [%s]\n",dlerror());
		exit(1);
	}
	// clear errors...
	dlerror();
	psin=(double(*)(double))dlsym(handle, "sin");
	char* err=dlerror();
	if(err!=NULL) {
		fprintf(stderr,"error in dlsym [%s]\n",err);
		exit(1);
	}

}
void init(void) __attribute__((constructor));

static void fini(void) {
	TRACE("in here");
	sc(dlclose(handle));
}
void fini(void) __attribute__((destructor));

extern "C" double sin(double x) {
	return psin(x)*2;
}
