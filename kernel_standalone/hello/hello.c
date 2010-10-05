#include <linux/kernel.h>
#include <linux/module.h>

/*
 *      This is a minimal driver doing just init and cleanup
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// parameters for this module

// constants for this module

// our own functions
static int __init mod_init(void) {
	printk(KERN_DEBUG "in init");
	return(0);
}


static void __exit mod_exit(void) {
	printk(KERN_DEBUG "in exit");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
