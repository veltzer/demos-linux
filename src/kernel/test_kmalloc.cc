#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

#include"us_helper.hh"
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
