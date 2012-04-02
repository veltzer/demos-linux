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

/*
* This example shows how to write a function that only works for constants.
* This file is a C file because g++ does not have this feature yet.
* A quick check in the internet shows that a patch to have this in g++ is on the way.
* Notice that the trick is to create a compile time error and is based on returning
* a void* instead of an int.
*/
static inline int _sum(int i) {
	return(i *(i + 1) / 2);
}

#define sum(i)\
	__builtin_choose_expr(\
		__builtin_constant_p(i),\
		_sum(i),\
		(void*)0)

/*
* This inline version does not work because of bug in gcc whereby __builtin_constant_p(i) is not
* a constant
*/

/*
static __attribute__((always_inline)) int sum(int i) {
	return __builtin_choose_expr(__builtin_constant_p(i),_sum(i),(void*)0);
}
*/

int main(int argc,char** argv,char** envp) {
	printf("sum for 100 is %d\n",sum(100));
	printf("sum for 5 is %d\n",sum(5));
	// these next 2 lines will create a compile time error...
	//volatile int j=25;
	//printf("sum for j is %d\n",sum(j));
	return(0);
}
