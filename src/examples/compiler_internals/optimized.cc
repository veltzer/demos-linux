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

/*
* This example shows that the compiler is very aggresive on optimization.
* It doesn't even unroll the loop! It simply calculates (1+99)*99/2=4950
* and puts the value in the code.
* Conclusion: DO NOT UNDERESTIMATE THE COMPILER!
*/

int main(int argc,char** argv,char** envp) {
	unsigned int sum=0;
	for(unsigned int i=0;i<100;i++) {
		sum+=i;
		//__sync_synchronize();
	}
	printf("sum is %u\n",sum);
	return EXIT_SUCCESS;
}
