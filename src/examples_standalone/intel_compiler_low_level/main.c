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
 * This example shows the synchronization primitives of the intel compiler
 */

int main(int argc,char** argv,char** envp) {
	// don't remove the prints, they make the disassembly easier to understand...
	printf("===================================\n");

	// this is a compiler reordering barrier in the intel compiler...
	// this does not emit any assembly...
	__memory_barrier();

	printf("===================================\n");

	// this is a machine reordering barrier, which is also a compiler barrier
	// (because the documentation says it is, not because every machine barrier
	// is also a compiler reordering barrier)
	__sync_synchronize();

	printf("===================================\n");

	// here is a compare and swap that succeeds...
	int a=4;
	int old_val=__sync_val_compare_and_swap(&a,4,5);
	printf("old_val is %d and a is %d\n",old_val,a);

	printf("===================================\n");

	// here is a compare and swap that fails...
	a=17;
	old_val=__sync_val_compare_and_swap(&a,4,5);
	printf("old_val is %d and a is %d\n",old_val,a);

	printf("===================================\n");
	return EXIT_SUCCESS;
}
