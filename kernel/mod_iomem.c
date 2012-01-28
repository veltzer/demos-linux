//#define DEBUG
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

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *      This is a driver to walk the iomem list and see all the resources...
 *
 *      Most of the code for this example is from:
 *      $KERNEL_SOURCE/kernel/resource.c
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// our own functions
static int __init mod_init(void) {
	struct resource *rp;

	PR_INFO("start");
	rp = iomem_resource.child;
	while (rp) {
		printk("  %0*llx-%0*llx\n",
		       8, (unsigned long long)rp->start,
		       8, (unsigned long long)rp->end
		       );
		// advance to the next resource...
		rp = rp->sibling;
	}
	return(0);
}


static void __exit mod_exit(void) {
	PR_INFO("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
