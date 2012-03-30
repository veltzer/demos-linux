//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/device.h> // for dev_[warn|info|..]
#include<linux/fs.h> // for fops

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

/*
 *	This is a driver which prints stuff at open and release and allows you
 *	to explore how open and close in user space translate to open and release
 *	calls in the kernel
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module with open and release");

// static data
static struct device* my_device;

// fops

/*
 * The open implementation. Currently this does nothing
 */
static int kern_open(struct inode *inode, struct file *filp) {
	dev_info(my_device,"start");
	return(0);
}

/*
 * The release implementation. Currently this does nothing
 */
static int kern_release(struct inode *inode, struct file *filp) {
	dev_info(my_device,"start");
	return(0);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.open=kern_open,
	.release = kern_release,
};

#include"device.inc"
