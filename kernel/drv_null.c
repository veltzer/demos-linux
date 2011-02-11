#include <linux/module.h> // for the MODULE_* stuff

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Showing how the kernel prints NULL pointers");

/*
 * TODO:
 * - show a snipplet here of how the printing shows up in the kernel log.
 */

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
