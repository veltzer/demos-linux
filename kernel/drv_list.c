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

/*
 *      Kernel module that demostrates manipulation list structures in the kernel
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Kernel module that demostrates manipulation list structures in the kernel");

// parameters for this module

static int chrdev_alloc_dynamic = 1;
module_param(chrdev_alloc_dynamic, bool, 0444);
MODULE_PARM_DESC(chrdev_alloc_dynamic, "Allocate the device number dynamically?");

static int first_minor = 0;
module_param(first_minor, int, 0444);
MODULE_PARM_DESC(first_minor, "first minor to allocate in dynamic mode (usually best to keep at 0)");

static int kern_major = 253;
module_param(kern_major, int, 0444);
MODULE_PARM_DESC(kern_major, "major to allocate in static mode");

static int kern_minor = 0;
module_param(kern_minor, int, 0444);
MODULE_PARM_DESC(kern_minor, "minor to allocate in static mode");

// constants for this module

// number of files we expose via the chr dev
static const int MINORS_COUNT = 1;

int register_dev(void);
void unregister_dev(void);

// List stuff
void *capi_list_create(void);
void capi_list_destroy(void *ptr);
int capi_list_isempty(void *lptr);
void *capi_list_iter_init(void *lptr);
int capi_list_iter_isover(void *lptr, void *iter);
void *capi_list_iter_getval(void *lptr, void *iter);
void *capi_list_iter_next(void *lptr, void *iter);
void capi_list_iter_del(void *lptr, void *iter);
void *capi_list_pop(void *lptr);
void capi_list_add(void *lptr, void *ptr);
void capi_list_print(void *lptr);

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

// first the structures

struct kern_dev {
	// pointer to the first device number allocated to us
	dev_t first_dev;
	// cdev structures for the char devices we expose to user space
	struct cdev cdev;
};

// static data
static struct kern_dev *pdev;
static const char      *name = "demo";
static struct class    *my_class;
static struct device   *my_device;

// now the functions

/*
 * This is the ioctl implementation. Currently this function supports
 * getting the image rows and columns
 */
void *lptr;
static int kern_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	int res;
	void *p;

	DEBUG("start");
	switch (cmd) {
	/* Create the list */
	case 0:
		lptr = capi_list_create();
		return(0);

		break;

	case 1:
		capi_list_destroy(lptr);
		return(0);

		break;

	case 2:
		res = capi_list_isempty(lptr);
		DEBUG("res is %d", res);
		return(0);

		break;

	case 3:
		p = capi_list_pop(lptr);
		DEBUG("p is %d", (int)p);
		return(0);

		break;

	case 4:
		capi_list_add(lptr, (void *)arg);
		return(0);

		break;

	case 5:
		capi_list_print(lptr);
		return(0);

		break;
	}
	return(0);
}


/*
 * The open implementation. Currently this does nothing
 */
static int kern_open(struct inode *inode, struct file *filp) {
	DEBUG("start");
	return(0);
}


/*
 * The release implementation. Currently this does nothing
 */
static int kern_release(struct inode *inode, struct file *filp) {
	DEBUG("start");
	return(0);
}


/*
 * The read implementation. Currently this does nothing.
 */
static ssize_t kern_read(struct file *filp, char __user *buf, size_t count, loff_t *pos) {
	DEBUG("start");
	return(0);
}


/*
 * The write implementation. Currently this does nothing.
 */
static ssize_t kern_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos) {
	DEBUG("start");
	return(0);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner   = THIS_MODULE,
	.open    = kern_open,
	.release = kern_release,
	.read    = kern_read,
	.write   = kern_write,
	.ioctl   = kern_ioctl,
};

int register_dev() {
	// create a class
	my_class = class_create(THIS_MODULE, MYNAME);
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
		if (alloc_chrdev_region(&pdev->first_dev, first_minor, MINORS_COUNT, myname)) {
			DEBUG("cannot alloc_chrdev_region");
			goto goto_dealloc;
		}
	} else {
		pdev->first_dev = MKDEV(kern_major, kern_minor);
		if (register_chrdev_region(pdev->first_dev, MINORS_COUNT, myname)) {
			DEBUG("cannot register_chrdev_region");
			goto goto_dealloc;
		}
	}
	DEBUG("allocated the device");
	// create the add the sync device
	cdev_init(&pdev->cdev, &my_fops);
	pdev->cdev.owner = THIS_MODULE;
	pdev->cdev.ops = &my_fops;
	kobject_set_name(&pdev->cdev.kobj, MYNAME);
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
	        name,
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


void unregister_dev() {
	device_destroy(my_class, pdev->first_dev);
	cdev_del(&pdev->cdev);
	unregister_chrdev_region(pdev->first_dev, MINORS_COUNT);
	kfree(pdev);
	class_destroy(my_class);
}


#include <linux/list.h>

/*
 *      List handling functions
 */

/*
 *      private structure to hold void pointers
 */
typedef struct _list_struct {
	struct list_head list;
	void             *ptr;                                         /* this is my specific data */
} list_struct;

/*
 *      Create a list
 */
void *capi_list_create(void) {
	struct list_head *lp = kmalloc(sizeof(struct list_head), GFP_KERNEL);

	INIT_LIST_HEAD(lp);
	return(lp);
}


/*
 *      Destroy a list
 *      TODO: need to clean the list...
 */
void capi_list_destroy(void *lptr) {
	struct list_head *lp = (struct list_head *)lptr;

	kfree(lp);
}


/*
 *      Is empty call
 */
int capi_list_isempty(void *lptr) {
	struct list_head *lp = (struct list_head *)lptr;

	return(list_empty(lp));
}


/*
 *      Init an iterator
 */
void *capi_list_iter_init(void *lptr) {
	struct list_head *lp = (struct list_head *)lptr;

	return(lp->next);
}


/*
 *      Is the iterator over
 */
int capi_list_iter_isover(void *lptr, void *iter) {
	struct list_head *lp = (struct list_head *)lptr;
	struct list_head *li = (struct list_head *)iter;

	return(li == lp);
}


/*
 *      Get the current value from the current iterator
 */
void *capi_list_iter_getval(void *lptr, void *iter) {
	//struct list_head* lp=(struct list_head*)lptr;
	struct list_head *li = (struct list_head *)iter;
	list_struct      *entry = list_entry(li, struct _list_struct, list);

	return(entry->ptr);
}


/*
 *      Advance the iterator one step
 */
void *capi_list_iter_next(void *lptr, void *iter) {
	//struct list_head* lp=(struct list_head*)lptr;
	struct list_head *li = (struct list_head *)iter;

	return(li->next);
}


/*
 *      Delete an element at the current position of the iterator
 */
void capi_list_iter_del(void *lptr, void *iter) {
	//struct list_head* lp=(struct list_head*)lptr;
	struct list_head *li = (struct list_head *)iter;
	list_struct      *entry = list_entry(li, struct _list_struct, list);

	kfree(entry);
}


/*
 *      Pop an element and return its pointer
 */
void *capi_list_pop(void *lptr) {
	struct list_head *lp = (struct list_head *)lptr;
	struct list_head *prev = lp->prev;
	list_struct      *entry;
	void             *ret;

	if (list_empty(lp)) {
		DEBUG("ERROR - pop should not be called on an empty list");
		return(NULL);
	} else {
		entry = list_entry(prev, struct _list_struct, list);
		ret = entry->ptr;
		list_del(prev);
		kfree(entry);
		return(ret);
	}
}


/*
 *      Add a pointer
 */
void capi_list_add(void *lptr, void *ptr) {
	struct list_head *lp = (struct list_head *)lptr;
	list_struct      *entry = kmalloc(sizeof(list_struct), GFP_KERNEL);

	entry->ptr = ptr;
	list_add(&entry->list, lp);
}


/*
 *      Print the list
 */
void capi_list_print(void *lptr) {
	struct list_head *lp = (struct list_head *)lptr;
	void             *item;
	void             *iter = capi_list_iter_init(lp);

	while (!capi_list_iter_isover(lp, iter)) {
		item = capi_list_iter_getval(lp, iter);
		DEBUG("item is %d", (int)item);
		iter = capi_list_iter_next(lp, iter);
	}
}
