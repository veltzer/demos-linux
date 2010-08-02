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
#include <linux/pagemap.h>
#include <linux/version.h>

#include "kernel_helper.h"
#include "ioctls.h"

/*
 *      This is a driver that maps memory allocated by the kernel into user space.
 *      The method is a user drive ioctl.
 *
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
static const char      *name = "demo";
static struct class    *my_class;
static struct device   *my_device;

// do we want to use kmalloc or get_free_pages ?
static bool do_kmalloc = true;
static void *alloc_mem(unsigned int size) {
	// kernel address to be returned
	void *kaddr;
	// order of allocation in case we use get_free_pages
	int order;

	DEBUG("start");
	if (do_kmalloc) {
		kaddr = kmalloc(size, GFP_KERNEL);
	} else {
		order = get_order(size);
		kaddr = (void *)__get_free_pages(GFP_KERNEL, order);
	}
	if (((unsigned int)kaddr) % PAGE_SIZE != 0) {
		return(NULL);
	}
	return(kaddr);
}


static void free_mem(void *kptr, unsigned int size) {
	// order of allocation in case we use get_free_pages
	int order;

	DEBUG("start");
	if (do_kmalloc) {
		// kfree does not return error code
		kfree(kptr);
	} else {
		order = get_order(size);
		// free pages does not return error code
		free_pages((unsigned long)kptr, order);
	}
}


static unsigned long map_to_user(struct file *filp, void *kptr, unsigned int size) {
	// the user space address to be returned
	unsigned long uptr;
	// the mmap struct to hold the semaphore of
	struct mm_struct *mm;
	// flags to pass to do_mmap_pgoff
	unsigned long flags;
	// old value in private field
	void *oldval;

	// print some debug info...
	DEBUG("size is (d) %d", size);

	mm = current->mm;
	// must NOT add MAP_LOCKED to the flags (it causes a hang)
	flags = MAP_POPULATE | MAP_SHARED;
	//flags=MAP_POPULATE|MAP_PRIVATE;
	flags &= ~(MAP_EXECUTABLE | MAP_DENYWRITE);
	down_write(&mm->mmap_sem);
	oldval = filp->private_data;
	filp->private_data = kptr;
	uptr = do_mmap_pgoff(
	        filp,                                                                                                                      /* file pointer for which filp->mmap will be called */
	        0,                                                                                                                         /* address - this is the address we recommend for user space - best not to ... */
	        size,                                                                                                                      /* size */
	        PROT_READ | PROT_WRITE,                                                                                                    /* protection */
	        //PROT_READ, /* protection */
	        flags,                                                                                                                     /* flags */
	        0                                                                                                                          /* pg offset */
	        );
	filp->private_data = oldval;
	up_write(&mm->mmap_sem);
	if (IS_ERR_VALUE(uptr)) {
		ERROR("ERROR: problem calling do_mmap_pgoff");
	} else {
		DEBUG("addr for user space is (lu) %lu / (p) %p", uptr, (void *)uptr);
	}
	return(uptr);
}


/*
 * static int unmap(unsigned long uadr,unsigned int size) {
 *      int res=do_munmap(current->mm,uadr,size);
 *      if(res) {
 *              ERROR("ERROR: unable to do_munmap");
 *      }
 *      return res;
 * }
 */

// now the functions

/*
 * This is the ioctl implementation. Currently this function supports
 * getting the image rows and columns
 */
// This is the kernel space pointer, this is all the kernel needs to know
void *kptr = NULL;
// for the size we get from user space
unsigned int size = -1;
// for the pointer we return to user space
unsigned long uptr = -1;
static int kern_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	// for results from functions
	int res;

	DEBUG("start with cmd %d", cmd);
	switch (cmd) {
	/*
	 *      Asking the kernel to mmap into user space.
	 *
	 *      Only argument is size.
	 */
	case IOCTL_DEMO_MAP:
		DEBUG("trying to mmap");
		size = arg;
		kptr = alloc_mem(size);
		if (kptr == NULL) {
			ERROR("ERROR: could not allocate memory");
			return(-EFAULT);
		}
		DEBUG("After alloc_mem with kptr=%p", kptr);
		uptr = map_to_user(filp, kptr, size);
		if (IS_ERR_VALUE(uptr)) {
			ERROR("ERROR: quiting on process of mmaping");
			return(-EFAULT);
		}
		DEBUG("After map_to_user");
		DEBUG("Successful exit");
		return(uptr);

		break;

	/*
	 *      Asking the kernel to munmap user space.
	 *
	 *      Not arguments are required
	 */
	case IOCTL_DEMO_UNMAP:
		DEBUG("trying to munmap");
		res = do_munmap(current->mm, uptr, size);
		if (res) {
			return(res);
		}
		DEBUG("After unmap");
		free_mem(kptr, size);
		DEBUG("Successful exit");
		// so we won't accidentaly use these pointers
		kptr = NULL;
		size = -1;
		uptr = -1;
		return(0);

		break;

	/*
	 *      Asking the kernel to write to the buffer
	 *
	 *      One argument which is the value to write
	 */
	case IOCTL_DEMO_WRITE:
		if (kptr == NULL) {
			ERROR("ERROR: kptr is NULL?!?");
			return(-EFAULT);
		}
		memset(kptr, arg, size);
		return(0);

		break;

	/*
	 *      Asking the kernel to check that the buffer
	 *      is a certain value
	 *
	 *      One argument which is the value to check
	 */
	case IOCTL_DEMO_READ:
		if (kptr == NULL) {
			ERROR("ERROR: kptr is NULL?!?");
			return(-EFAULT);
		}
		return(memcheck(kptr, arg, size));

		break;

	/*
	 *      Asking the kernel to copy the in kernel buffer
	 *      to user space
	 *
	 *      One argument which is the pointer to the user space
	 *      buffer
	 */
	case IOCTL_DEMO_COPY:
		if (kptr == NULL) {
			ERROR("ERROR: kptr is NULL?!?");
			return(-EFAULT);
		}
		return(copy_to_user((void *)arg, kptr, size));

		break;
	}
	return(-EFAULT);
}


/*
 *      VMA ops
 */
static void kern_vma_open(struct vm_area_struct *vma) {
	DEBUG("start");
	DEBUG("vma->start is %lu , %lx", vma->vm_start, vma->vm_start);
	DEBUG("vma->end-vma->start is %lu", vma->vm_end - vma->vm_start);
}


static void kern_vma_close(struct vm_area_struct *vma) {
	DEBUG("start");
	DEBUG("vma->start is %lu , %lx", vma->vm_start, vma->vm_start);
	DEBUG("vma->end-vma->start is %lu", vma->vm_end - vma->vm_start);
}


// on error should return VM_FAULT_SIGBUS
static int kern_vma_fault(struct vm_area_struct *vma, struct vm_fault *vmf) {
	struct page *page = NULL;
	// kernel side address
	void *kaddr;
	// offset at which user wants the page...
	unsigned long offset;

	DEBUG("start");
	offset = (unsigned long)vmf->virtual_address - vma->vm_start;
	kaddr = vma->vm_private_data;
	kaddr += offset;
	page = virt_to_page(kaddr);
	if (page == NULL) {
		ERROR("couldnt find page");
		return(VM_FAULT_SIGBUS);
	}
	get_page(page);
	vmf->page = page;
	return(0);
}


static struct vm_operations_struct kern_vm_ops = {
	.open  = kern_vma_open,
	.close = kern_vma_close,
	.fault = kern_vma_fault,
};

/*
 * The mmap implementation.
 */
static int kern_mmap(struct file *filp, struct vm_area_struct *vma) {
	/*
	 *      // size of memory to map
	 *      unsigned int size;
	 *      // for the physical address
	 *      unsigned long phys;
	 *      // for the starting page number
	 *      unsigned int pg_num;
	 *      // for return values
	 *      int ret;
	 *
	 *      size=vma->vm_end-vma->vm_start;
	 *      phys=virt_to_phys(kadr);
	 *      pg_num=phys >> PAGE_SHIFT;
	 *      DEBUG("size is %d",size);
	 *      DEBUG("kadr is %p",kadr);
	 *      DEBUG("phys is %lx",phys);
	 *      DEBUG("pg_num is %d",pg_num);
	 *      DEBUG("vm_start is %lx",vma->vm_start);
	 *      DEBUG("vm_end is %lx",vma->vm_end);
	 *      DEBUG("vm_pgoff is %lx",vma->vm_pgoff);
	 *      ret=remap_pfn_range(
	 *              vma, // into which vma
	 *              vma->vm_start, // where in the vma
	 *              pg_num, // which starting physical page
	 *              size, // how much to map (in bytes)
	 *              vma->vm_page_prot // what protection to give
	 *      );
	 *      if(ret) {
	 *              ERROR("ERROR: could not remap_pfn_range");
	 *              return ret;
	 *      }
	 */
	// pointer for already allocated memory
	void *kadr;

	DEBUG("start");
	kadr = filp->private_data;
	vma->vm_flags |= VM_RESERVED;

	vma->vm_private_data = kadr;
	vma->vm_ops = &kern_vm_ops;
	kern_vma_open(vma);
	DEBUG("all ok from mmap. returning");
	return(0);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.ioctl = kern_ioctl,
	.mmap  = kern_mmap,
};

static int register_dev(void) {
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
#if LINUX_VERSION_CODE == 132634                     /* target kernel */
	my_device = device_create(
	        my_class,                                                                                                                          /* our class */
	        NULL,                                                                                                                              /* device we are subdevices of */
	        pdev->first_dev,
	        "%s",
	        name
	        );
#else /* ubuntu kernel */
	my_device = device_create(
	        my_class,                                                                                                                   /* our class */
	        NULL,                                                                                                                   /* device we are subdevices of */
	        pdev->first_dev,
	        NULL,
	        "%s",
	        name
	        );
#endif
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");
