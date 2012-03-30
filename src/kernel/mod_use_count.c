//#define DEBUG
#include<linux/module.h> // for MODULE_*, module_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for device support

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module with device file to demo use count");

// static data
static struct device* my_device;

// fops

/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
};

#include"device.inc"
