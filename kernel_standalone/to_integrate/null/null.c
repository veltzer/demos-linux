#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>

/*
	This module shows how to write a character device similar in functinality
	to the /dev/null device...
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A /dev/null similar driver");

// a hardcoded value for the major number.
// this is filled using cat /proc/devices and looking for a number
// which isn't used...
static int null_major=200;
module_param(null_major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(null_major, "the major number this device will use for registration");
static int null_minor=0;
module_param(null_minor, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(null_minor, "the minor number this device will use for registration");

// the name of this driver to be found in /proc/devices after registration
const char* null_name="null";

// the name of the device node to be created
const char* null_device_name="mynull";

// device dev_t for this driver
static dev_t null_dev;

// device for this driver...
static struct device* null_device;

// class for this driver...
static struct class *null_class;

// this is the null write operation - it simply does nothing and reports
// that the entire count bytes were written...
static ssize_t null_write(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
{
	printk("Writing %d bytes...\n",count);
	*ppos+=count;
	return count;
}

static const struct file_operations null_fops = {
	.write=null_write,
};

static int __init null_init(void) {
	printk("Starting...\n");

	// register the
	if (register_chrdev(null_major,null_name,&null_fops)) {
		printk("unable to get major %d\n",null_major);
		goto err_finish;
	}
	printk("Registered chrdev %d...\n",null_major);
	null_dev=MKDEV(null_major,null_minor);
	null_class=class_create(THIS_MODULE,null_name);
	if(IS_ERR(null_class)) {
		printk("Error creating the class...\n");
		goto err_chrdev;
	}
	printk("Created the class...\n");
	null_device=device_create(null_class, NULL,
			null_dev,NULL,
			null_device_name);
	if(null_device==NULL) {
		printk("Error creating the device...\n");
		goto err_class;
	}
	printk("Created the device...\n");

	printk("Finished insmod successfully\n");
	return 0;
	err_class:
		class_destroy(null_class);
	err_chrdev:
		unregister_chrdev(null_major,null_name);
	err_finish:
		return -EFAULT;
}

static void __exit null_exit(void) {
	printk("Ending...\n");
	device_destroy(null_class,null_dev);
	class_destroy(null_class);
	unregister_chrdev(null_major,null_name);
}

module_init(null_init);
module_exit(null_exit);
