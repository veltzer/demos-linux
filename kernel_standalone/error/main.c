#include <linux/module.h>

/*
 * This is a module with error in the initialisation process...
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Hello World module");

// parameters for this module

// constants for this module

// our own functions
static int __init mod_init(void) {
	printk(KERN_ALERT "in init");
	return(-1);
}


static void __exit mod_exit(void) {
	printk(KERN_ALERT "in exit");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
