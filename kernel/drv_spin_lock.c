#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Showing how to use spin locks in the kernel");

/*
 * For a full description of which types of spin locks to use where please refere
 * to Rusty Russel's "Unreliable Guide to Kernel Locking" which comes with the kernel
 * documentation within the kernel sources or here:
 * http://www.kernel.org/pub/linux/kernel/people/rusty/kernel-locking/index.html
 */

// parameters for this module

static int chrdev_alloc_dynamic = 1;
static int first_minor = 0;
static int kern_major = 253;
static int kern_minor = 0;

// constants for this module

// number of files we expose via the chr dev
static const int MINORS_COUNT = 1;

// first the structures

struct kern_dev {
	// pointer to the first device number allocated to us
	dev_t first_dev;
	// cdev structures for the char devices we expose to user space
	struct cdev cdev;
};

// static data
static struct kern_dev *pdev;
static struct class* my_class;
static struct device* my_device;

// now the functions

/*
 * This is the ioctl implementation.
 */
spinlock_t mr_lock = SPIN_LOCK_UNLOCKED;
spinlock_t *lock_t;
static int kern_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	unsigned long flags;

	DEBUG("start");
	switch (cmd) {
	case 0:
		// lock - this will disable interrupts on the local CPU Only!!!
		spin_lock_irqsave(&mr_lock, flags);
		/* critical section ... */
		spin_unlock_irqrestore(&mr_lock, flags);
		// lock - this will not disable interrupts and may cause a dead lock if you
		// try to acquire the same lock from an interrupt handler or higher level task
		spin_lock(&mr_lock);
		spin_unlock(&mr_lock);
		break;

	case 1:
		lock_t = kmalloc(sizeof(spinlock_t), GFP_KERNEL);
		spin_lock_init(lock_t);
		spin_lock(lock_t);
		spin_unlock(lock_t);
		kfree(lock_t);
		break;
	}
	return(0);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.ioctl = kern_ioctl,
};

int register_dev(void) {
	// create a class
	my_class = class_create(THIS_MODULE, THIS_MODULE->name);
	if (IS_ERR(my_class)) {
		goto goto_nothing;
	}
	DEBUG("created the class");
	// alloc and zero
	pdev = kmalloc(sizeof(struct kern_dev), GFP_KERNEL);
	if (pdev == NULL) {
		goto goto_destroy;
	}
	memset(pdev, 0, sizeof(struct kern_dev));
	DEBUG("set up the structure");
	if (chrdev_alloc_dynamic) {
		if (alloc_chrdev_region(&pdev->first_dev, first_minor, MINORS_COUNT, THIS_MODULE->name)) {
			DEBUG("cannot alloc_chrdev_region");
			goto goto_dealloc;
		}
	} else {
		pdev->first_dev = MKDEV(kern_major, kern_minor);
		if (register_chrdev_region(pdev->first_dev, MINORS_COUNT, THIS_MODULE->name)) {
			DEBUG("cannot register_chrdev_region");
			goto goto_dealloc;
		}
	}
	DEBUG("allocated the device");
	// create the add the sync device
	cdev_init(&pdev->cdev, &my_fops);
	pdev->cdev.owner = THIS_MODULE;
	pdev->cdev.ops = &my_fops;
	kobject_set_name(&pdev->cdev.kobj, THIS_MODULE->name);
	if (cdev_add(&pdev->cdev, pdev->first_dev, 1)) {
		DEBUG("cannot cdev_add");
		goto goto_deregister;
	}
	DEBUG("added the device");
	// now register it in /dev
	my_device = device_create(
		my_class,/* our class */
		NULL,/* device we are subdevices of */
		pdev->first_dev,
		NULL,
		THIS_MODULE->name,
		0
	);
	if (my_device == NULL) {
		DEBUG("cannot create device");
		goto goto_create_device;
	}
	DEBUG("did device_create");
	return(0);

	//goto_all:
	//	device_destroy(my_class,pdev->first_dev);
goto_create_device:
	cdev_del(&pdev->cdev);
goto_deregister:
	unregister_chrdev_region(pdev->first_dev, MINORS_COUNT);
goto_dealloc:
	kfree(pdev);
goto_destroy:
	class_destroy(my_class);
goto_nothing:
	return(-1);
}


void unregister_dev(void) {
	device_destroy(my_class, pdev->first_dev);
	cdev_del(&pdev->cdev);
	unregister_chrdev_region(pdev->first_dev, MINORS_COUNT);
	kfree(pdev);
	class_destroy(my_class);
}


// our own functions
static int __init mod_init(void) {
	return(register_dev());
}


static void __exit mod_exit(void) {
	unregister_dev();
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
