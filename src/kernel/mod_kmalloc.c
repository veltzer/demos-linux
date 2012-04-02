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

//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/fs.h> // for fops
#include<linux/slab.h> // for the kmalloc API
#include<linux/pci.h> // for pci_alloc_consistent

#include"shared.h" // for the ioctl numbers

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A module to explore kmalloc behaviour");

// static data
static struct device* my_device;

// fops

/*
* This is the ioctl implementation.
*/
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	void* ptr = NULL;
	unsigned long addr = -1;
	dma_addr_t dma_handle;
	unsigned long size;

	//int res;
	PR_DEBUG("start");
	switch (cmd) {
	/*
	* kmalloc function.
	*
	* One argument which is the size to allocate
	*/
	case IOCTL_DEMO_KMALLOC:
		size = arg * PAGE_SIZE;
		ptr = kmalloc(GFP_KERNEL, size);
		if (ptr == NULL) {
			PR_ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			PR_ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		kfree(ptr);
		ptr = NULL;
		return(0);
	/*
	* __get_free_pages function.
	*
	* One argument which is the size to allocate
	*/
	case IOCTL_DEMO_GET_FREE_PAGES:
		size = arg * PAGE_SIZE;
		addr = __get_free_pages(GFP_KERNEL, get_order(size));
		if (addr == 0) {
			//if(IS_ERR_VALUE(addr)) {
			PR_ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		if (addr % PAGE_SIZE != 0) {
			PR_ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		free_pages(addr, get_order(size));
		PR_DEBUG("addr is %lx, mod is %ld", addr, addr % PAGE_SIZE);
		addr = -1;
		return(0);
	/*
	* PCI allocation function
	*/
	case IOCTL_DEMO_PCI_ALLOC_CONSISTENT:
		size = arg * PAGE_SIZE;
		ptr = pci_alloc_consistent(NULL, size, &dma_handle);
		if (ptr == NULL) {
			PR_ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			PR_ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		pci_free_consistent(NULL, size, ptr, dma_handle);
		ptr = NULL;
		return(0);
	case IOCTL_DEMO_DMA_ALLOC_COHERENT:
		size = arg * PAGE_SIZE;
		ptr = dma_alloc_coherent(my_device, size, &dma_handle, GFP_KERNEL);
		if (ptr == NULL) {
			PR_ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			PR_ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		dma_free_coherent(my_device, size, ptr, dma_handle);
		ptr = NULL;
		return(0);
	}
	return(-EINVAL);
}

/*
* The file operations structure.
*/
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_unlocked_ioctll,
};

#include"device.inc"
