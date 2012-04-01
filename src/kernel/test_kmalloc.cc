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

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

#include<us_helper.h>
#include"shared.h"

/*
 * This test explores how kmalloc behaves when given various
 * sizes to allocate
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d;

	//klog_clear();
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	printf("starting out\n");
	for (unsigned int i = 1; i < 1000000; i += 100) {
		// kmalloc does not allocate on a page boundry...
		//CHECK_NOT_M1(ioctl(d,IOCTL_DEMO_KMALLOC,i),"kmalloc");
		//printf("kmalloc: i is %d\n",i);
		CHECK_NOT_M1(ioctl(d, IOCTL_DEMO_GET_FREE_PAGES, i));
		printf("__get_free_pages: i is %d\n", i);
		// I don't seem to be able to call these unless I have a real
		// device (like PCI etc...).
		//CHECK_NOT_M1(ioctl(d,IOCTL_DEMO_PCI_ALLOC_CONSISTENT,i),"pci_alloc_consistent");
		//printf("pci_alloc_consistent: i is %d",i);
		//CHECK_NOT_M1(ioctl(d,IOCTL_DEMO_DMA_ALLOC_COHERENT,i),"dma_alloc_coherent");
		//printf("dma_alloc_coherent: i is %d",i);
	}
	CHECK_NOT_M1(close(d));
	return(0);
}
