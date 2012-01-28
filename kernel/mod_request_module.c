//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/printk.h> // for the printk API
/*
#include <linux/kernel.h>
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
*/

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module demos how to insmod one module from another");

// our own functions
static int __init mod_init(void) {
	PR_DEBUG("start");
	if (request_module("crc7") > 0) {
		printk("looks bad\n");
	} else {
		printk("looks ok\n");
	}
	return(0);
}

static void __exit mod_exit(void) {
	PR_DEBUG("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
