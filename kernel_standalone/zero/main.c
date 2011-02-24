#include <linux/module.h> // for MODULE_*, module_*
#include <linux/fs.h> // for fops
#include <linux/device.h> // for class_create
#include <linux/slab.h> // for kmalloc
#include <asm/uaccess.h> // for copy_to_user, access_ok

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A simple implementation for something like /dev/zero");

/*
 * This is an implementation of a kernel module which exposes a /dev/zero like
 * device to user space. Whenever someone reads from the device the buffer passed
 * to kernel gets filled with zeros.
 * If you want to see the real implementation in the kernel see:
 * $KERNEL_SOURCES/drivers/char/mem.c
 *
 * TODO:
 * - move to dynamic registration of majors and mniors.
 * - handle errors better
 * - use clear user as does mem.c to clear the user space buffer.
 * - use schedule like mem.c to allow for scheduling (explain why that
 *   is in the notes).
 */

// notice this hardcoded major number (not good!!!)
const int ZERO_MAJOR=190;
const int ZERO_MINOR=0;

// these are the actual operations

static int open_zero(struct inode * inode, struct file * file) {
	char* p=(char*)kmalloc(PAGE_SIZE, GFP_KERNEL);
	memset(p,0,PAGE_SIZE);
	file->private_data=p;
	INFO("all is ok and buffer is %p",p);
	return 0;
}

static ssize_t read_zero(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	ssize_t old_count;
	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;

	old_count=count;
	while(count) {
		size_t curr;
		if(count>PAGE_SIZE) {
			curr=PAGE_SIZE;
		} else {
			curr=count;
		}
		if(copy_to_user(buf,file->private_data,curr)) {
			return -EFAULT;
		} else {
			buf+=curr;
			count-=curr;
		}
	}
	*ppos+=old_count;
	return old_count;
}

int release_zero(struct inode* inode,struct file* file) {
	kfree(file->private_data);
	return 0;
}

// this is the operations table
static const struct file_operations zero_fops = {
	.open=open_zero,
	.read=read_zero,
	.release=release_zero,
};

// this variable will store the class
static struct class *my_class;

static int zero_init(void) {
	// this is registering the new device operations
	if(register_chrdev(ZERO_MAJOR,THIS_MODULE->name,&zero_fops))
		printk(KERN_ERR "unable to get major %d for %s dev\n",ZERO_MAJOR,THIS_MODULE->name);
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		unregister_chrdev(ZERO_MAJOR, THIS_MODULE->name);
		printk(KERN_ERR "failed to create class\n");
	}
	// and now lets auto-create a /dev/ node
	device_create(my_class, NULL, MKDEV(ZERO_MAJOR, ZERO_MINOR),"%s",THIS_MODULE->name);
	return 0;
}

static void zero_exit(void) {
	device_destroy(my_class, MKDEV(ZERO_MAJOR, ZERO_MINOR));
	class_destroy(my_class);
	unregister_chrdev(ZERO_MAJOR,THIS_MODULE->name);
}

module_init(zero_init);
module_exit(zero_exit);
