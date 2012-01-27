#define DEBUG
#include <linux/module.h> // for MODULE_*, module_*
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC
#include <linux/slab.h> // for kmalloc
#include <linux/cdev.h> // for char device
#include <linux/fs.h> // for fops
#include <linux/device.h> // for device support

#include "kernel_helper.h" // our own helper

/*
 *	Minimal driver that supports ioctl.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Minimal driver that supports ioctl");

// first the structures

struct kern_dev {
	// pointer to the first device number allocated to us
	dev_t first_dev;
	// cdev structures for the char devices we expose to user space
	struct cdev cdev;
};

// static data
static struct kern_dev* pdev;
static struct class* my_class;
static struct device* my_device;

// now the functions

/*
 * This is the ioctl implementation.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	PR_DEBUG("start");
	return(0);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_unlocked_ioctll,
};

#include "device.inc"
