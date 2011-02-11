#include <linux/module.h> // for the MODULE_* stuff

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module showing how to use BUG()");

/*
 *	Take note of the following facts:
 *	- after a driver does a BUG() it cannot be unloaded (reboot will be needed).
 *	- BUG() generates a snipplet in the kernel log that can be used to report
 *		bugs to kernel maintainers.
 *	- BUG() generates a stack trace that you can use for debugging.
 */
static int __init mod_init(void) {
	BUG();
	return(0);
}

static void __exit mod_exit(void) {
}

module_init(mod_init);
module_exit(mod_exit);
