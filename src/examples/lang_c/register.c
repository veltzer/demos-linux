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
#include<stdlib.h> // for EXIT_SUCCESS

/*
* An example of doing a loop and telling the compiler that a register should be
* involved.
* We will also print the register.
* How did we know what register the compiler used as the iterator of the loop? We
* disassembled the code.
* Note that removing the 'volatile' will cause the code to stop running correctly.
* (need to look at the disassembly to find out exactly why...).
* Here is a funny side note: if you remove the printf for i then the compiler
* will not optimize the loop away because of the printing of the register. But
* you will see that the value of the register does go down from 100 to 1 (not
* from 99 to 0 mind you). The compiler thinks that its better to count down
* than up because of the shorter "compare to zero" at the end of the loop.
*/

int main(int argc,char** argv,char** envp) {
	unsigned int register i;
	for(i=0;i<100;i++) {
		printf("i is %d\n",i);
		int reg;
		asm volatile("movl %%ebx, %0;":"=r"(reg));
		printf("reg is %d\n",reg);
	}
	return EXIT_SUCCESS;
}
