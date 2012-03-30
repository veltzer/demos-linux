//#define DEBUG
#include<linux/module.h> // for module_*, MODULE_*
#include<linux/fs.h> // for fops definitions
#include<linux/device.h> // for struct device

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

#include"shared.h" // for ioctl numbers

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Showing how to return error codes from kernel to use space");

/*
 *      This driver explores how to correctly return error code from kernel code
 *      and what happens in user space.
 */

// static data
static struct device* my_device;

// fops

/*
 * This is the ioctl implementation. This returns whatever it gets.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	PR_DEBUG("start");
	return(arg);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner   = THIS_MODULE,
	.unlocked_ioctl   = kern_unlocked_ioctll,
};

#include"device.inc"
