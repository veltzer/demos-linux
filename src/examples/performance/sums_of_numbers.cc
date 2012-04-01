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
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example demostrates that the compiler actually adds full loops
 * of numbers at compile time and avoids making the loops at runtime at
 * all... Pretty good! (g++ 4.4.3)
 *
 * Why is this important? Because you may be very confused when debugging
 * an application and seeing that much of the code that you write just isn't
 * there. The wisdom derived here is never underestimate the compilers wisdom
 * to throw your code away or to morph it to something which will be very hard
 * to recognize.
 *
 * Use a disassembler to see the resulting machine code.
 *
 * TODO:
 * - make this example disassemble itself and show what the compiler did, or maybe
 * morph it's own code to show that the sum is there ?
 */

int main(int argc,char** argv,char** envp) {
	int sum;
	sum=0;
	for(int i=0;i<100;i++) {
		sum+=i;
	}
	printf("sum is %d\n",sum);
	sum=0;
	for(int i=99;i>=0;i--) {
		sum+=i;
	}
	printf("sum is %d\n",sum);
	sum=0;
	for(int i=0;i<100;i+=2) {
		sum+=i;
	}
	printf("sum is %d\n", sum);
	sum=0;
	for(int i=98;i>=0;i-=2) {
		sum+=i;
	}
	printf("sum is %d\n", sum);
	return EXIT_SUCCESS;
}
