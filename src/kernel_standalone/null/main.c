#include <linux/module.h> // for MODULE_*, module_*
#include <linux/stat.h> // for permission bits on module_param
#include <linux/fs.h> // for fops
#include <linux/device.h> // for device_create
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC...

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A simple implementation for something like /dev/null");

/*
 * This is a module implementing something like /dev/null with hardcoded major number
 * It can optionally register itself both as a class and in /dev.
 * TODO:
 * - allocate the major and minors dynamically
 */

static int auto_register=1;
module_param(auto_register, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(auto_register, "Do you want me to auto register the file in /dev ?");

// notice this hardcoded major number (not good!!!)
const int NULL_MAJOR=190;
const int NULL_MINOR=0;

// these are the actual operations

static ssize_t write_null(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	//DEBUG("start");
	// remmember to increment the position to let the user think that he is actually
	// writing something...
	*ppos+=count;
	return count;
}

// this is the operations table
static const struct file_operations null_fops = {
	.write=write_null,
};

// this variable will store the class
static struct class *my_class;
static struct device *my_device;

static int null_init(void) {
	int err;
	pr_info("start");
	// this is registering the new device operations
	if((err=register_chrdev(NULL_MAJOR,THIS_MODULE->name,&null_fops))) {
		pr_err("unable to get major %d for %s dev",NULL_MAJOR,THIS_MODULE->name);
		goto err_nothing;
	}
	if(auto_register) {
		// this is creating a new class (/sys/class)
		my_class=class_create(THIS_MODULE,THIS_MODULE->name);
		if(IS_ERR(my_class)) {
			pr_err("failed to create class");
			err=PTR_ERR(my_class);
			goto err_register;
		}
		pr_info("created class");
		// and now lets auto-create a /dev/ node
		my_device=device_create(my_class, NULL, MKDEV(NULL_MAJOR, NULL_MINOR),"%s",THIS_MODULE->name);
		if(IS_ERR(my_device)) {
			pr_err("failed to create device");
			err=PTR_ERR(my_device);
			goto err_class;
		}
		pr_info("emitted message to udev to create /dev file in user space");
	}
	pr_info("device loaded successfuly...");
	return 0;
// err_device:
	device_destroy(my_class, MKDEV(NULL_MAJOR, NULL_MINOR));
err_class:
	class_destroy(my_class);
err_register:
	unregister_chrdev(NULL_MAJOR, THIS_MODULE->name);
err_nothing:
	return err;
}

static void null_exit(void) {
	pr_info("start");
	if(auto_register) {
		device_destroy(my_class, MKDEV(NULL_MAJOR, NULL_MINOR));
		class_destroy(my_class);
	}
	unregister_chrdev(NULL_MAJOR,THIS_MODULE->name);
	pr_info("device unloaded successfuly...");
}

module_init(null_init);
module_exit(null_exit);
