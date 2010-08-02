#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>

#include "kernel_helper.h"

/*
 *      This demo shows how to use BUG().
 *
 *      Take note of the following facts:
 *      - after a driver does a BUG() it cannot be unloaded (reboot will be needed).
 *      - BUG() generates a snipplet in the kernel log that can be used to report
 *      bugs to kernel maintainers.
 *      - BUG() generates a stack trace that you can use for debugging.
 */
static int __init mod_init(void) {
	BUG();
	return(0);
}


static void __exit mod_exit(void) {
}


module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");
