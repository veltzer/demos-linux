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
#include <linux/err.h>

#define DO_DEBUG
#include "kernel_helper.h" // our own helper

static int __init mod_init(void) {
	unsigned long ptr1 = 15;
	unsigned long ptr2 = -15;

	DEBUG("IS_ERR_VALUE(%lu) is %ld\n", ptr1, IS_ERR_VALUE(ptr1));
	DEBUG("IS_ERR_VALUE(%lu) is %ld\n", ptr2, IS_ERR_VALUE(ptr2));
	return(0);
}


static void __exit mod_exit(void) {
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);

// some module meta data

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");
