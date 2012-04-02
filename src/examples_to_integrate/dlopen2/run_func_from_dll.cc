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
#include<stdio.h> // for fprintf(3), printf(3), perror(3)
#include<dlfcn.h> // for dlopen(3), dlsym(3), dlerror(3), dlclose(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, exit(3)
#include<errno.h> // for errno

int main(int argc,char** argv,char** envp) {
	if(argc!=4) {
		fprintf(stderr,"%s: usage: %s [library] [function] [value]\n",argv[0],argv[0]);
		return EXIT_FAILURE;
	}
	const char* p_lib=argv[1];
	const char* p_func=argv[2];
	const char* p_sval=argv[3];
	const double p_dval=atof(p_sval);
	void* h=dlopen(p_lib,RTLD_NOW);
	if(h==NULL) {
		fprintf(stderr,"error with dlopen: %s\n",dlerror());
		exit(errno);
	}
	void* sym=dlsym(h,p_func);
	if(sym==NULL) {
		fprintf(stderr,"error with dlsym: %s\n",dlerror());
		exit(errno);
	}
	double (*f)(double)=(typeof(f))sym;
	double result=f(p_dval);
	printf("result is %lf\n",result);
	int rc=dlclose(h);
	if(rc) {
		fprintf(stderr,"error with dlclose: %s\n",dlerror());
		exit(errno);
	}
	return EXIT_SUCCESS;
}
