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

/*
* This example shows how to step out of the array bounds, in this case
* in order to step over some other variable. Note that if you do not
* make the other number (in this case 'num') materialize by taking a reference
* to it then it will not materialize and a bug will not be seen (although a bug
* will occur!).
*/

int main(int argc,char** argv,char** envp) {
	int a[10];
	int num=1995;
	printf("%p %p\n",a,&num);
	int i;
	for(i=0;i<10;i++) {
		a[i]=i;
	}
	for(i=0;i<10;i++) {
		printf("a[%d]=%d\n",i,a[i]);
	}
	printf("num is %d\n",num);
	// next line will not compile if warnings in gcc
	// are turned on since gcc is smart
	//a[10]=2011;
	for(i=0;i<10;i++) {
		printf("a[%d]=%d\n",i,a[i]);
	}
	printf("num is %d\n",num);
	return 0;
}
