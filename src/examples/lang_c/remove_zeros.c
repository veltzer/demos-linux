/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for scanf(3), printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_INT()

void print_array(int* ar, int size) {
	int i;
	for(i=0; i<size; i++)
		printf("ar[%d]=%d\n", i, ar[i]);
}

int remove_zeros(int* ar, int size) {
	int pos=0;
	int i;
	for(i=0; i<size; i++) {
		if(ar[i]!=0)
			ar[pos++]=ar[i];
	}
	return pos;
}

int main(int argc, char** argv, char** envp) {
	// read the size of the array from the user...
	int size;
	printf("give me the size of the array: ");
	CHECK_INT(scanf("%d", &size), 1);

	// read the array from the user...
	int ar[size];
	int i;
	for(i=0; i<size; i++) {
		printf("give me the %d element: ", i);
		// scanf("%d",ar+i);
		CHECK_INT(scanf("%d", &ar[i]), 1);
	}
	// print the array before...
	print_array(ar, size);
	// remove the zeros...
	int newsize=remove_zeros(ar, size);
	// print the array after...
	printf("*************** results after *********************\n");
	print_array(ar, newsize);
	return EXIT_SUCCESS;
}
