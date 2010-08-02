#include <linux/kernel.h>
#include <linux/module.h>
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

#include "kernel_helper.h"

/*
 *      This example demos how to allocate and deallocate coherent memoery for DMA
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// parameters for this module

// constants for this module

// our own functions

const unsigned int size = 1024 * 1024 * 24;
//dma_addr_t device_addr;
unsigned int device_addr;
void         *vptr;

#define DO_LOOP

static int __init mod_init(void) {
#ifdef DO_LOOP
	int i;
#endif // DO_LOOP
	DEBUG("start");
#ifdef DO_LOOP
	for (i = 0; i < 1000; i++) {
		vptr = dma_alloc_coherent(NULL, size, &device_addr, GFP_KERNEL | GFP_DMA);
		dma_free_coherent(NULL, size, vptr, device_addr);
	}
#else // DO_LOOP
	vptr = dma_alloc_coherent(NULL, size, &device_addr, GFP_KERNEL | GFP_DMA);
	printk("vptr is %p\n", vptr);
	printk("size is %d\n", size);
	printk("device_addr is %d\n", device_addr);
#endif // DO_LOOP
	return(0);
}


static void __exit mod_exit(void) {
	DEBUG("start");
#ifndef DO_LOOP
	dma_free_coherent(NULL, size, vptr, device_addr);
#endif // DO_LOOP
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
