#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include "kernel_helper.h"

/*
 *      This is a test to see how the kernel prints NULL pointers...
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// parameters for this module

// constants for this module

// our own functions
static int __init mod_init(void) {
	DEBUG("start");
	DEBUG("NULL is %p", NULL);
	return(0);
}


static void __exit mod_exit(void) {
	DEBUG("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
