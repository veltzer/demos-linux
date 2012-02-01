//#define DEBUG
#include <linux/module.h> // for MODULE_*. module_*
#include <linux/printk.h> // for pr_*

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 * Which module name is better to use: KBUILD_MODNAME or THIS_MODULE->name?
 * Probably KBUILD_MODNAME. Look at this:

mark@abel:~/install/linux-3.1.6/drivers$ fgrep "KBUILD_MODNAME" `find . -type f -name "*.c"` | wc -l
586
mark@abel:~/install/linux-3.1.6/drivers$ fgrep "THIS_MODULE->name" `find . -type f -name "*.c"` | wc -l
1
mark@abel:~/install/linux-3.1.6/drivers$ 

 * This sort of solves the issue.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A module showing how to refer to the name of the module you are in");

// init/exit functions
static int __init mod_init(void) {
	pr_info("this module is called " KBUILD_MODNAME);
	pr_info("this module is called %s",KBUILD_MODNAME);
	pr_info("this module is called %s",THIS_MODULE->name);
	return(0);
}

static void __exit mod_exit(void) {
}

// declaration of init/cleanup functions of this module
module_init(mod_init);
module_exit(mod_exit);
