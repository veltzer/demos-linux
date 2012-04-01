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

#include<link.h> // for dl_iterate_phdr(3)
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows how to find out which libraries you are linked to.
 * Sort of a cheap ldd replacement.
 */
static bool verbose=false;

static int callback(struct dl_phdr_info *info, size_t size, void *data) {
	printf("name=%s (%d segments)\n", info->dlpi_name, info->dlpi_phnum);
	if(verbose) {
		for(int j=0;j<info->dlpi_phnum;j++)
			printf("\t\t header %2d: address=%10p\n", j, (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
	}
	return 0;
}

int main(int argc, char** argv,char** envp) {
	dl_iterate_phdr(callback, NULL);
	return EXIT_SUCCESS;
}
