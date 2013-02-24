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
#include <stdio.h>
#include <setjmp.h>

// This buffer saves the context of the jump. We need one for each unique
// point we wish to jump to
static jmp_buf env;

/* this simulates a function which sometimes encounters errors */
void func() {
	static int counter=0;
	counter++;
	if(counter%3==0) {
		longjmp(env,counter);
	}
	fprintf(stderr,"this is the continuation of the function\n");
}

int main(int argc,char** argv,char** envp) {
	for(int c=0;c<10;c++) {
		int ret;
		ret=setjmp(env);
		if(!ret) {
			// This is the regular code. We get here when setting doing the
			// setjmp for the first time
			fprintf(stderr,"c is %d\n",c);
			func();
		} else {
			// we got here from longjmp, this means that some
			// error occured
			fprintf(stderr,"oops, got an error from func %d\n",ret);
		}
	}
	return 0;
}
