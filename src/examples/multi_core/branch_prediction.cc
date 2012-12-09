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

#include <firstinclude.h>
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

// these we stolen shamelessly from the kernel.
// It is a good idea to use them so that if you have a compiler
// that does not have __builtin_expect you would just define likely/unlikely
// in a way that is appropriate to that compiler, or maybe even just
// the identity function.
#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
//#define likely(x) x
//#define unlikely(x) x

/*
* This is an example of giving hints to the compiler about branchings.
* Note that:
* - some cpus have branch prediction in hardware and ignore such hints.
* - you can pass flags to the compiler to ignore these hints and create
*	regular branch instructions.
* - some compiler and/or compiler versions do not actually use this information.
*
* Where can this help?
* It can allow the compiler to reorder the control-flow graph to reduce the number of branches taken for the 'likely' path. This can have a marked improvement in loops where you're checking multiple exit cases.
*
* TODO:
* - on the command line using time(1) this example runs the same with branch prediction and without.
*	Modify this to have a marked effect on performance and measure itself so that I can show when
*	this is effective.
*/

int main(int argc,char** argv,char** envp) {
	int x=0;
	long long sum=0;
	while(likely(x<1000000000)) {
	// mispredict on purpose...
	//while(unlikely(x<1000000000)) {
		// to force the compiler to actually DO the loop.
		// moving this line to sum+=x will actually mean that the compiler
		// will calculate the whole loop in advance!!!
		sum+=x*x;
		x++;
	}
	printf("sum is %lld\n",sum);
	printf("x is %d\n",x);
	return EXIT_SUCCESS;
}
