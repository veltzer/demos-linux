//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/gfp.h>
#include <linux/pagemap.h>

#include "shared.h"

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

/*
 *      A driver which allocates memory. This is intended to explore kmalloc
 *      behaviour...
 */

// static data
static struct device* my_device;

// fops

/*
 * This is the ioctl implementation.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	void          *ptr = NULL;
	unsigned long addr = -1;
	dma_addr_t dma_handle;
	unsigned long size;

	//int res;
	PR_DEBUG("start");
	switch (cmd) {
	/*
	 *      kmalloc function.
	 *
	 *      One argument which is the size to allocate
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
	 *      __get_free_pages function.
	 *
	 *      One argument which is the size to allocate
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
	 *      PCI allocation function
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

#include "device.inc"
