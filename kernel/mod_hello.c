#define DEBUG
#include <linux/module.h> // for MODULE_*. module_*
#include <linux/printk.h> // for pr_*

#include "kernel_helper.h" // our own helper

/*
 *      This is a minimal driver doing just init and cleanup
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// our own functions
static int __init mod_init(void) {
	pr_debug("init");
	PR_INFO("init");
	return(0);
}


static void __exit mod_exit(void) {
	PR_INFO("exit");
	pr_debug("exit");
}

// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
