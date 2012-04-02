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
#include<stdio.h> // for printf(3), scanf(3), perror(3)
#include<stdlib.h> // for rand(3), EXIT_SUCCESS
#include<errno.h> // for errno

int main(int argc,char** argv,char** envp) {
	int rand_number;
	int cont=1;
	while(cont) {
		rand_number=rand()%100;
		//printf("rand_number is %d\n",rand_number);
		int guess=-1;
		while(guess!=rand_number) {
			printf("what is your guess ? ");
			int ret=scanf("%d",&guess);
			if(ret!=1) {
				perror("scanf");
				return -1;
			}
			if(guess<rand_number) {
				printf("guess higher...\n");
			}
			if(guess>rand_number) {
				printf("guess lower...\n");
			}
			if(guess==rand_number) {
				printf("RIGHT!!!\n");
			}
		}
		printf("want to play more ? ");
		int ret=scanf("%d",&cont);
		if(ret!=1) {
			int err=errno;
			perror("scanf");
			return err;
		}
	}
	return EXIT_SUCCESS;
}
