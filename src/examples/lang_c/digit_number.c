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
#include<stdio.h> // for printf(3), scanf(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include<us_helper.h> // for CHECK_INT()

int digit_num(long int l) {
	int digits;
	if(l<0) {
		digits=1;
		l=-l;
	} else {
		digits=0;
	}
	while(l>0) {
		l/=10;
		digits++;
	}
	return digits;
}

long int get_number() {
	long int ret;
	CHECK_INT(scanf("%ld",&ret),1);
	return ret;
}

int main(int argc,char** argv,char** envp) {
	printf("enter the first number: ");
	long int num1=get_number();
	printf("enter the second number: ");
	long int num2=get_number();
	long int mul=num1*num2;
	printf("num1 is %ld, %d\n",num1,digit_num(num1));
	printf("num2 is %ld, %d\n",num2,digit_num(num2));
	printf("mul is %ld, %d\n",mul,digit_num(mul));
	return EXIT_SUCCESS;
}
