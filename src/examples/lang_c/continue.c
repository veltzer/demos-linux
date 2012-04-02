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
 * Compare the version that uses the 'continue' keyword with the version that
 * does not. Which one is shorter? more maintanable? easier to read? easier
 * to change? Sometimes continue is just the right answer. In kernel coding even
 * more so...
 */

int main(int argc,char** argv,char** envp) {
	/* without continue... */
	printf("first round...\n");
	int i;
	for(i=0;i<100;i++) {
		if(i%5!=0) {
			if((i+1)%3==0) {
				printf("i is %d\n",i);
			}
		}
	}
	/* with continue... */
	printf("second round...\n");
	for(i=0;i<100;i++) {
		if(i%5==0) continue;
		if((i+1)%3!=0) continue;
		printf("i is %d\n",i);
	}
	return 0;
}
