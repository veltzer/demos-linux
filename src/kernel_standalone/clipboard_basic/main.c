#include<linux/module.h>		/* Module API */
#include<linux/moduleparam.h>		/* Module parameters API */
#include<linux/slab.h>			/* Memory alloc functions */
#include<linux/fs.h>			/* File operations */
#include<linux/errno.h>			/* Error codes */
#include<linux/uaccess.h>		/* User space access */
#include<linux/printk.h>		/* printk and pr_* API */
#include<linux/cdev.h>			/* Character device */
#include<linux/device.h>		/* for class_create */

MODULE_AUTHOR("Mark Veltzer");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A in kernel buffer exercise");

// static data
// the actual buffer in kernel space...
static char* buffer=NULL;
// what is the size of a buffer?
static const unsigned int BUFFER_SIZE=4096;
// how many buffers do we have? (files in /dev...)
static const unsigned int BUFFER_COUNT=1;
// this is the first dev_t allocated to us...
static dev_t clipboard_dev;
// this variable will hold our cdev struct
static struct cdev* clipboard_cdev;
// this variable will store the class
static struct class *my_class;
// this is our device...
struct device* clipboard_device;

/*
 * Open the device. Optional.
 */
int clipboard_open(struct inode* inode,struct file* filp) {
	pr_debug("device is open");
	return 0;
}

/*
 * Release the device. Optional as well.
 */
int clipboard_release(struct inode* inode,struct file* filp) {
	pr_debug("device is released");
	return 0;
}

/*
 * Simply copy data to the user buffer...
 */
ssize_t clipboard_read(struct file* filp,__user char* user_buf,size_t count,loff_t* offset) {
	int ret; // error code in case of error
	int bytes_to_read;
	/* Number of bytes left to read in the open file */
	bytes_to_read=min(BUFFER_SIZE-(*offset),(loff_t)count);
	if(bytes_to_read==0) {
		/* All read and returning 0 for End Of File */
		return 0;
	}
	if((ret=copy_to_user(
		user_buf,
		*offset+buffer,
		bytes_to_read
	))) {
		return ret;
	} else {
		*offset+=bytes_to_read;
		return bytes_to_read;
	}
}

/*
 * Simply copy the data from the user buffer...
 */
ssize_t clipboard_write(struct file* filp,const char* user_buf,size_t count,loff_t* offset) {
	int ret; // error code in case of error
	int bytes_to_write=min(BUFFER_SIZE-(*offset),(loff_t)count);
	// buffer is full, return error 
	if(bytes_to_write==0) {
		return(-EIO);
	}
	// now we have room in the clipboard
	/* we don't need this code (short write implementation)
	// if the user wants to write more than we have space for
	if(count>bytes_to_write) {
		// Can't write beyond the end of the device
		return(-EIO);
	}
	*/
	if((ret=copy_from_user(buffer+*offset,user_buf,bytes_to_write))) {
		return ret;
	} else {
		*offset+=bytes_to_write;
		return bytes_to_write;
	}
}

/* our file operations structure that gathers all the ops */

struct file_operations clipboard_fops={
	.owner = THIS_MODULE,
	.open=clipboard_open,
	.release=clipboard_release,
	.read=clipboard_read,
	.write=clipboard_write
};

/*
 * Module housekeeping.
 */
static int __init clipboard_init(void) {
	int ret; // error code to return in case of error
	buffer=kmalloc(BUFFER_SIZE,GFP_KERNEL);
	if(!buffer) {
		pr_err("kmalloc");
		ret=-ENOMEM;
		goto any_error;
	}
	memset(buffer,0,BUFFER_SIZE);

	if((ret=alloc_chrdev_region(&clipboard_dev,0,BUFFER_COUNT,THIS_MODULE->name))) {
		pr_err("alloc_chrdev_region");
		goto error_after_alloc;
	}

	clipboard_cdev=cdev_alloc();
	if(IS_ERR(clipboard_cdev)) {
		pr_err("cdev_alloc");
		ret=PTR_ERR(clipboard_cdev);
		goto error_after_region;
	}
	clipboard_cdev->ops=&clipboard_fops;
	clipboard_cdev->owner=THIS_MODULE;

	if((ret=cdev_add(clipboard_cdev,clipboard_dev,BUFFER_COUNT))) {
		/* Only if we allocated a cdev but did not register do we
		* we need to kfree it. In any other case cdev_del is enough */
		pr_err("cdev_add");
		kfree(clipboard_cdev);
		goto error_after_region;
	}

	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		pr_err("class_create");
		ret=PTR_ERR(my_class);
		goto error_after_register;
	}
	// create the /dev file entry...
	clipboard_device=device_create(my_class, NULL, clipboard_dev,NULL,"%s",THIS_MODULE->name);
	if(IS_ERR(clipboard_device)) {
		pr_err("device_create");
		ret=PTR_ERR(clipboard_device);
		goto error_after_class_create;
	}
	pr_info("clipboard loaded sucessfuly");
	return 0;

/*
error_after_device_create:
	device_destroy(my_class, clipboard_dev);
*/
error_after_class_create:
	class_destroy(my_class);
error_after_register:
	cdev_del(clipboard_cdev);
error_after_region:
	unregister_chrdev_region(clipboard_dev,BUFFER_COUNT);
error_after_alloc:
	kfree(buffer);
any_error:
	return ret;
}

static void __exit clipboard_cleanup(void) {
	device_destroy(my_class, clipboard_dev);
	class_destroy(my_class);
	cdev_del(clipboard_cdev);
	unregister_chrdev_region(clipboard_dev,BUFFER_COUNT);
	kfree(buffer);
	pr_info("clipboard unloaded succefully.");
}

module_init(clipboard_init);
module_exit(clipboard_cleanup);
