//#define DEBUG
#include <linux/module.h> // for MODULE_*. module_*
#include <linux/printk.h> // for pr_*

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *      This is a minimal module showing various logging functions 
 *      I show only kernel logging functions.
 *      Always prefer the pr_* functions, or better yet, the dev_* functions.
 *      TODO:
 *      show rate limited functions here (see printk.h for details).
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module for showing logging functions");

void print_error_once_wrapper(void) {
	pr_err_once("this is an error that will not be repeated");
}

// our own functions
static int __init mod_init(void) {
	printk(KERN_EMERG "this is an emergency message");
	printk(KERN_ALERT "this is an alert message");
	printk(KERN_CRIT "this is a critical message");
	printk(KERN_ERR "this is a err message");
	printk(KERN_WARNING "this is a warning message");
	printk(KERN_NOTICE "this is a notice message");
	printk(KERN_INFO "this is an info message");
	printk(KERN_DEBUG "this is a debug message");
	printk(FW_BUG "this is a firmware bug message");
	printk(FW_WARN "this is a firmware warning message");
	printk(FW_INFO "this is a firmware info message");
	printk(HW_ERR "this is a hardware error message");
	pr_emerg("this is an emergency message");
	pr_alert("this is an alert message");
	pr_crit("this is a critical message");
	pr_err("this is an error message");
	pr_warning("this is a warning message");
	pr_warn("same as above");
	pr_notice("this is a notice message");
	pr_info("this is an info message");
	pr_devel("this is a development message");
	pr_debug("this is a debug message");
	print_error_once_wrapper();
	return(0);
}


static void __exit mod_exit(void) {
	// this will not show any output...
	print_error_once_wrapper();
}

// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
