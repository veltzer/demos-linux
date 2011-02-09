#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/device.h>

/*
 * TODO:
 * - remove lots of unneeded includes.
 * - handle errors better
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A simple implementation for something like /dev/null");

// notice this hardcoded major number (not good!!!)
const int NULL_MAJOR=190;
const int NULL_MINOR=0;
const char* myname="null";

// these are the actual operations

static int open_null(struct inode * inode, struct file * filp) {
	return 0;
}

static ssize_t read_null(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	return 0;
}

static ssize_t write_null(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	return count;
}

// this is the operations table
static const struct file_operations null_fops = {
	.read=read_null,
	.write=write_null,
	.open=open_null,
};

// this variable will store the class
static struct class *my_class;

static int null_init(void) {
	// this is registering the new device operations
	if(register_chrdev(NULL_MAJOR,"null",&null_fops))
		printk("unable to get major %d for %s dev\n",NULL_MAJOR,myname);
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,myname);
	if(IS_ERR(my_class))
		printk("failed to create class\n");
	// and now lets auto-create a /dev/ node
	device_create(my_class, NULL, MKDEV(NULL_MAJOR, NULL_MINOR),"newnull");
	return 0;
}

static void null_exit(void) {
	device_destroy(my_class, MKDEV(NULL_MAJOR, NULL_MINOR));
	class_destroy(my_class);
	unregister_chrdev(NULL_MAJOR, myname);
}

module_init(null_init);
module_exit(null_exit);
