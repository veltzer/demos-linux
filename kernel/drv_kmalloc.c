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
#include <linux/mman.h>
#include <linux/gfp.h>
#include <linux/pagemap.h>

#include "ioctls.h"

#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *      A driver which allocates memory. This is intended to explore kmalloc
 *      behaviour...
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
static struct class    *my_class;
static struct device   *my_device;

/*
 * This is the ioctl implementation.
 */
static long kern_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	void          *ptr = NULL;
	unsigned long addr = -1;
	dma_addr_t dma_handle;
	unsigned long size;

	//int res;
	DEBUG("start");
	switch (cmd) {
	/*
	 *      kmalloc function.
	 *
	 *      One argument which is the size to allocate
	 */
	case IOCTL_DEMO_KMALLOC:
		size = arg * PAGE_SIZE;
		ptr = kmalloc(GFP_KERNEL, size);
		if (ptr == NULL) {
			ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		kfree(ptr);
		ptr = NULL;
		return(0);

		break;

	/*
	 *      __get_free_pages function.
	 *
	 *      One argument which is the size to allocate
	 */
	case IOCTL_DEMO_GET_FREE_PAGES:
		size = arg * PAGE_SIZE;
		addr = __get_free_pages(GFP_KERNEL, get_order(size));
		if (addr == 0) {
			//if(IS_ERR_VALUE(addr)) {
			ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		if (addr % PAGE_SIZE != 0) {
			ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		free_pages(addr, get_order(size));
		DEBUG("addr is %lx, mod is %ld", addr, addr % PAGE_SIZE);
		addr = -1;
		return(0);

		break;

	/*
	 *      PCI allocation function
	 */
	case IOCTL_DEMO_PCI_ALLOC_CONSISTENT:
		size = arg * PAGE_SIZE;
		ptr = pci_alloc_consistent(NULL, size, &dma_handle);
		if (ptr == NULL) {
			ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		pci_free_consistent(NULL, size, ptr, dma_handle);
		ptr = NULL;
		return(0);

		break;

	case IOCTL_DEMO_DMA_ALLOC_COHERENT:
		size = arg * PAGE_SIZE;
		ptr = dma_alloc_coherent(my_device, size, &dma_handle, GFP_KERNEL);
		if (ptr == NULL) {
			ERROR("unable to allocate %lu", size);
			return(-EFAULT);
		}
		addr = (unsigned int)ptr;
		if (addr % PAGE_SIZE != 0) {
			ERROR("page size issue with addr=%lu", addr);
			return(-EFAULT);
		}
		addr = -1;
		dma_free_coherent(my_device, size, ptr, dma_handle);
		ptr = NULL;
		return(0);

		break;
	}
	return(-EFAULT);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_ioctl,
};

static int register_dev(void) {
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
	        my_class,                                                                                                                   /* our class */
	        NULL,                                                                                                                       /* device we are subdevices of */
	        pdev->first_dev,
	        NULL,
	        "%s",
		THIS_MODULE->name
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


static void unregister_dev(void) {
	device_destroy(my_class, pdev->first_dev);
	cdev_del(&pdev->cdev);
	unregister_chrdev_region(pdev->first_dev, MINORS_COUNT);
	kfree(pdev);
	class_destroy(my_class);
}


static int __init mod_init(void) {
	return(register_dev());
}


static void __exit mod_exit(void) {
	unregister_dev();
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");
