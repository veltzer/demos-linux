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
#include <sys/utsname.h> // for uname(2)
#include <stdbool.h> // for bool
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for CHECK_NOT_M1(), __stringify()

/*
* This is a demo showing the sizes of variables on the architecture
* it is running on (it also prints the name of the architecture).
*
* This is intended for a GNU/linux system.
*
* TODO:
* Show the results inside this source code for 32bit and 64bit linux.
*/

#define PRINT_SIZEOF(type) printf("size of " __stringify(type) " is %d\n", sizeof(type));

int main(int argc,char** argv,char** envp) {
	struct utsname buf;

	CHECK_NOT_M1(uname(&buf));
	printf("architecture is [%s]\n", buf.machine);
	PRINT_SIZEOF(bool);
	PRINT_SIZEOF(char);
	PRINT_SIZEOF(unsigned char);
	PRINT_SIZEOF(short);
	PRINT_SIZEOF(unsigned short);
	PRINT_SIZEOF(int);
	PRINT_SIZEOF(unsigned int);
	PRINT_SIZEOF(long);
	PRINT_SIZEOF(unsigned long);
	PRINT_SIZEOF(long long);
	PRINT_SIZEOF(unsigned long long);
	PRINT_SIZEOF(float);
	PRINT_SIZEOF(double);
	// this is not allowed in C
	//float* x={ 5, 3, 2, 1 };
	// but this is...
	float x[]={ 5, 3, 2, 1 };
	printf("size of x is %d\n",sizeof(x));
	typedef struct _empty {
	} empty;
	PRINT_SIZEOF(empty);
	typedef struct _array {
		char a[0];
	} array;
	PRINT_SIZEOF(array);
	typedef struct _charintchar {
		char c1;
		int i;
		char c2;
	} charintchar;
	PRINT_SIZEOF(charintchar);
	return EXIT_SUCCESS;
}
