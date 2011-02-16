#include <linux/kernel.h>
#include <linux/module.h>

#include "another_file.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Multi file kernel module example");

// parameters for this module

// constants for this module

// our own functions
static int __init mod_init(void) {
	do_something_init();
	return(0);
}


static void __exit mod_exit(void) {
	do_something_exit();
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
