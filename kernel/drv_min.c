#include <linux/module.h> // for MODULE_*. module_*

#include "kernel_helper.h" // our own helper

/*
 *      This is a minimal driver doing just init and cleanup
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// our own functions
static int __init mod_init(void) {
	DEBUG("start");
	return(0);
}


static void __exit mod_exit(void) {
	DEBUG("start");
}

// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
