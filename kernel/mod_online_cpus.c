//#define DEBUG
#include <linux/module.h> // for MODULE_*. module_*
#include <linux/printk.h> // for pr_*
#include <linux/cpumask.h> // for num_online_cpus

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 * This is a minimal module for showing the number of current online cpus
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module showing the number of online cpus");

// our own functions
static int __init mod_init(void) {
	pr_info("number of online cpus is %d",num_online_cpus());
	return(0);
}


static void __exit mod_exit(void) {
}

// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
