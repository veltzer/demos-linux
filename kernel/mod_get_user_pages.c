#define DEBUG
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
#include <linux/pagemap.h>

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("get_user_pages demo");

/*
 *	This driver demostrates how to map user space (virtual space) to kernel space.
 *	You can either get it fragmented in pages, or if you really need to,
 *	use the MMU to map it to a single kernel side linear address...
 */

#include "shared.h"

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
static struct kern_dev* pdev;
static struct class* my_class;
static struct device* my_device;

// now the functions

/*
 * This is the ioctl implementation.
 */

// this is the size of the buffer
unsigned int size;
// this is the kernel space pointer (matches user)
void *ptr = NULL;
// this is the kernel space pointer (we got from vmap)
void *vptr = NULL;
// will store page data as they are mapped...
struct page **pages;
// will store the numer of pages required...
unsigned int nr_pages;


static inline void pages_unlock(void) {
	unsigned int i;

	// unlock the pages
	for (i = 0; i < nr_pages; i++) {
		unlock_page(pages[i]);
	}
}


static inline void pages_lock(void) {
	unsigned int i;

	// unlock the pages
	for (i = 0; i < nr_pages; i++) {
		lock_page(pages[i]);
	}
}


static inline void pages_dirty(void) {
	unsigned int i;

	// set the pages as dirty
	for (i = 0; i < nr_pages; i++) {
		SetPageDirty(pages[i]);
	}
}


static inline void pages_unmap(void) {
	unsigned int i;

	// set the pages as dirty
	for (i = 0; i < nr_pages; i++) {
		if (!PageReserved(pages[i])) {
			SetPageDirty(pages[i]);
		}
		page_cache_release(pages[i]);
	}
}


static inline void pages_reserve(void) {
	unsigned int i;

	// set the pages as reserved
	for (i = 0; i < nr_pages; i++) {
		SetPageReserved(pages[i]);
	}
}


static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	// this is the buffer which will hold the data of the buffer from user space...
	BufferStruct b;
	// for results from calls
	int res;
	// for printing buffers
	char *cptr;
	// for loop length
	unsigned int sloop;
	// for loops
	unsigned int i;
	// some unsigned ints for address manipulation...
	unsigned int bpointer, offset, aligned, newsize;
	// the newly created vma
	struct vm_area_struct *vma;

	PR_DEBUG("start with ioctl %u", cmd);
	switch (cmd) {
	/*
	 *	This is asking the kernel to map the memory to kernel space.
	 */
	case IOCTL_DEMO_MAP:
		// get the data from the user
		if (copy_from_user(&b, (void *)arg, sizeof(b))) {
			PR_ERROR("problem with copy_from_user");
			return(-EFAULT);
		}
		PR_DEBUG("after copy");
		bpointer = (unsigned int)b.pointer;
		offset = bpointer % PAGE_SIZE;
		aligned = bpointer - offset;
		newsize = b.size + offset;
		PR_DEBUG("bpointer is %x", bpointer);
		PR_DEBUG("offset is %u", offset);
		PR_DEBUG("aligned is %x", aligned);
		PR_DEBUG("newsize is %u", newsize);

		/*
		 * // make sure that the user data is page aligned...
		 * if(((unsigned int)b.pointer)%PAGE_SIZE!=0) {
		 *	PR_ERROR("pointer is not page aligned");
		 *	return -EFAULT;
		 * }
		 * PR_DEBUG("after modulu check");
		 */
		// find the number of pages
		nr_pages = (newsize - 1) / PAGE_SIZE + 1;
		PR_DEBUG("nr_pages is %d", nr_pages);
		// alocate page structures...
		if ((pages = kmalloc(nr_pages * sizeof(struct page *), GFP_KERNEL)) == NULL) {
			PR_ERROR("could not allocate page structs");
			return(-ENOMEM);
		}
		PR_DEBUG("after pages allocation");
		// get user pages and fault them in
		down_write(&current->mm->mmap_sem);
		// rw==READ means read from drive, write into memory area
		res = get_user_pages(
			current,
			current->mm,
			aligned,
			nr_pages,
			1,/* write */
			0,/* force */
			pages,
			NULL
		);
		vma = find_vma(current->mm, bpointer);
		vma->vm_flags |= VM_DONTCOPY;
		up_write(&current->mm->mmap_sem);
		PR_DEBUG("after get_user_pages res is %d", res);
		// Errors and no page mapped should return here
		if (res != nr_pages) {
			PR_ERROR("could not get_user_pages. res was %d", res);
			kfree(pages);
			return(-EFAULT);
		}
		//pages_lock();
		//pages_reserve();
		//pages_unlock();
		// map the pages to kernel space...
		vptr = vmap(pages, nr_pages, VM_MAP, PAGE_KERNEL);
		if (vptr == NULL) {
			PR_ERROR("could not get_user_pages. res was %d", res);
			kfree(pages);
			return(-EFAULT);
		}
		ptr = vptr + offset;
		size = b.size;
		PR_DEBUG("after vmap - vptr is %p", vptr);
		// free the pages
		kfree(pages);
		pages = NULL;
		PR_DEBUG("after freeing the pages");
		// were dont! return with success
		PR_DEBUG("success - on the way out");
		return(0);

		break;

	/*
	 *	This is asking the kernel to unmap the data
	 *	No arguments are passed
	 */
	case IOCTL_DEMO_UNMAP:
		// this function does NOT return an error code. Strange...:)
		vunmap(vptr);
		vptr = NULL;
		ptr = NULL;
		size = 0;
		nr_pages = 0;
		pages_unmap();
		return(0);

		break;

	/*
	 *	This is asking the kernel to read the data.
	 *	No arguments are passed
	 */
	case IOCTL_DEMO_READ:
		cptr = (char *)ptr;
		sloop = min(size, (unsigned int)10);
		PR_DEBUG("sloop is %d", sloop);
		for (i = 0; i < sloop; i++) {
			PR_DEBUG("value of %d is %c", i, cptr[i]);
		}
		return(0);

		break;

	/*
	 *	This is asking the kernel to write on our data
	 *	argument is the constant which will be used...
	 */
	case IOCTL_DEMO_WRITE:
		memset(ptr, arg, size);
		//pages_dirty();
		return(0);

		break;
	}
	return(-EINVAL);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_unlocked_ioctll,
};

static int register_dev(void) {
	// create a class
	my_class = class_create(THIS_MODULE, THIS_MODULE->name);
	if (IS_ERR(my_class)) {
		goto goto_nothing;
	}
	PR_DEBUG("created the class");
	// alloc and zero
	pdev = kmalloc(sizeof(struct kern_dev), GFP_KERNEL);
	if (pdev == NULL) {
		goto goto_destroy;
	}
	memset(pdev, 0, sizeof(struct kern_dev));
	PR_DEBUG("set up the structure");
	if (chrdev_alloc_dynamic) {
		if (alloc_chrdev_region(&pdev->first_dev, first_minor, MINORS_COUNT, THIS_MODULE->name)) {
			PR_DEBUG("cannot alloc_chrdev_region");
			goto goto_dealloc;
		}
	} else {
		pdev->first_dev = MKDEV(kern_major, kern_minor);
		if (register_chrdev_region(pdev->first_dev, MINORS_COUNT, THIS_MODULE->name)) {
			PR_DEBUG("cannot register_chrdev_region");
			goto goto_dealloc;
		}
	}
	PR_DEBUG("allocated the device");
	// create the add the sync device
	cdev_init(&pdev->cdev, &my_fops);
	pdev->cdev.owner = THIS_MODULE;
	pdev->cdev.ops = &my_fops;
	kobject_set_name(&pdev->cdev.kobj, THIS_MODULE->name);
	if (cdev_add(&pdev->cdev, pdev->first_dev, 1)) {
		PR_DEBUG("cannot cdev_add");
		goto goto_deregister;
	}
	PR_DEBUG("added the device");
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
		PR_DEBUG("cannot create device");
		goto goto_create_device;
	}
	PR_DEBUG("did device_create");
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


// our own functions
static int __init mod_init(void) {
	PR_DEBUG("start");
	return(register_dev());
}


static void __exit mod_exit(void) {
	PR_DEBUG("start");
	unregister_dev();
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
