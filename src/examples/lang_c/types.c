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

#include<stdio.h> // for printf(3)
// this must be defined if you are in a C++ context to get the constants
#define __STDC_LIMIT_MACROS
#include<stdint.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This shows how to define a long long constant and how to print long longs in linux.
 */
int main(int argc,char** argv,char** envp) {
	unsigned long long l1 = 214748364899LL;

	// this next line will not work (the LL is required...)
	//unsigned long long l1=214748364899;
	fprintf(stderr, "l1 is %llu\n", l1);
	long l2 = INT32_MAX;
	fprintf(stderr, "l2 is %ld\n", l2);
	unsigned long long l3 = UINT64_MAX;
	fprintf(stderr, "l3 is %llu\n", l3);
	long long l4 = INT64_MAX;
	fprintf(stderr, "l4 is %lld\n", l4);
	//long long max=9223372036854775807LL;
	fprintf(stderr, "sizeof(3.14) is %d\n", sizeof(3.14));
	fprintf(stderr, "sizeof(3.14f) is %d\n", sizeof(3.14f));
	//float f=1.6676;
	//double d=1.8988f;
	return EXIT_SUCCESS;
}
