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
#include <linux/bootmem.h>

#include "kernel_helper.h"

/*
 *      This is a driver that demos the alloc_bootmem function...
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// our own functions
static int __init mod_init(void) {
	void *p;

	DEBUG("start");
	// this will NOT work if your module is dynamic (you must be linked into the kernel)
	//alloc_bootmem(10000000);
	p = alloc_bootmem(PAGE_SIZE);
	if (p == NULL) {
		INFO("unable to allocate bootmem");
		return(-1);
	}
	return(0);
}


static void __exit mod_exit(void) {
	DEBUG("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
