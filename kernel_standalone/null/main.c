#include <linux/module.h>
#include <linux/stat.h>
#include <linux/ctype.h>
#include <linux/fs.h> // for fops
#include <linux/device.h>
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC...

#include "kernel_helper.h" // our own helper

/*
 * TODO:
 * - remove lots of unneeded includes.
 * - handle errors better
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A simple implementation for something like /dev/null");

static int auto_register=1;
module_param(auto_register, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(auto_register, "Do you want me to auto register the file in /dev ?");

// notice this hardcoded major number (not good!!!)
const int NULL_MAJOR=190;
const int NULL_MINOR=0;

// these are the actual operations

static int open_null(struct inode * inode, struct file * filp) {
	INFO("start");
	return 0;
}

static ssize_t read_null(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	INFO("start");
	return 0;
}

static ssize_t write_null(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	INFO("start");
	*ppos+=count;
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
	INFO("start");
	// this is registering the new device operations
	if(register_chrdev(NULL_MAJOR,THIS_MODULE->name,&null_fops)) {
		printk(KERN_ERR "unable to get major %d for %s dev\n",NULL_MAJOR,THIS_MODULE->name);
		return -1;
	}
	if(auto_register) {
		// this is creating a new class (/proc/devices)
		my_class=class_create(THIS_MODULE,THIS_MODULE->name);
		if(IS_ERR(my_class)) {
			unregister_chrdev(NULL_MAJOR, THIS_MODULE->name);
			printk(KERN_ERR "failed to create class\n");
			return PTR_ERR(my_class);
		}
		INFO("created class");
		// and now lets auto-create a /dev/ node
		device_create(my_class, NULL, MKDEV(NULL_MAJOR, NULL_MINOR),"%s",THIS_MODULE->name);
		INFO("emitted message to udev to create /dev file in user space");
	}
	INFO("device loaded successfuly...");
	return 0;
}

static void null_exit(void) {
	INFO("start");
	if(auto_register) {
		device_destroy(my_class, MKDEV(NULL_MAJOR, NULL_MINOR));
		class_destroy(my_class);
	}
	unregister_chrdev(NULL_MAJOR,THIS_MODULE->name);
	INFO("device unloaded successfuly...");
}

module_init(null_init);
module_exit(null_exit);
