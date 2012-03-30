//#define DEBUG
#include<linux/module.h> // for MODULE_*, module_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for struct device and dev_* functions

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("modules that shows how to use the dev_* debugging macros");

// static data
static struct device* my_device;

// fops

/*
 * The open implementation.
 */
static int kern_open(struct inode *inode, struct file *filp) {
	// you can use dev_printk like you would printk only with the added device...
	dev_printk(KERN_DEBUG,my_device,"this is my debug message");
	// or better yet, use the predefined ones:
	dev_emerg(my_device,"emergency");
	dev_alert(my_device,"alert");
	dev_crit(my_device,"critical");
	dev_err(my_device,"error");
	dev_warn(my_device,"warning");
	dev_notice(my_device,"notice");
	dev_info(my_device,"info");
	return(0);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open=kern_open,
};

#include"device.inc"
