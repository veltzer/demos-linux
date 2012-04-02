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
#include<stdbool.h> // for bool, true and false
#include<us_helper.h> // for __stringify()

/*
 * This is a demo of how to use the _Bool and stdbool.h headers for
 * boolean support in C code.
 *
 * Note that this code MUST be compiled using a C compiler and not a C++
 * compiler since the results will be different. In a C++ compiler,
 * _Bool is bool which is a built in type and true and false are built
 * ins.
 */
int main(int argc,char** argv,char** envp) {
	printf("sizeof(_Bool) is %d\n",sizeof(_Bool));
	printf("sizeof(bool) is %d\n",sizeof(bool));
	printf("_Bool is really %s\n",__stringify(_Bool));
	printf("bool is really %s\n",__stringify(bool));
	printf("true is really %s\n",__stringify(true));
	printf("false is really %s\n",__stringify(false));
	return(0);
}
