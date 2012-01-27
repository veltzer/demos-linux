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
#include <linux/mman.h>
#include <linux/pagemap.h>

#include "kernel_helper.h" // our own helper

/*
 *      This is a driver that maps memory allocated by the kernel into user space.
 *      The method is the regular mmap system call.
 *
 *      This driver also shows you how to implement mmap via a regular ioctl call.
 *
 *      TODO:
 *              - show how to kfree my pointer when munmap is made
 *              - separate the part of the driver that does the mmap via the ioctl call.
 */
unsigned long addr;
void          *vaddr;
char          *caddr;
unsigned int size;
//unsigned int size_rounded;
unsigned int pg_num;
unsigned long phys;
unsigned int pages;
//struct page* pp;
//void* start_addr;
bool do_kmalloc = true;
#define DO_FREE
#define DO_RESERVE

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

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
static struct class    *my_class;
static struct device   *my_device;

// now the functions

/*
 * This is the ioctl implementation.
 */
unsigned long addr;
int ioctl_size;
void          *kaddr;
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	//int i;
	char str[256];
	void         *ptr;
	unsigned int order;

	unsigned long private;
	unsigned long adjusted;
	unsigned int diff;
	int ret;
	struct vm_area_struct *vma;
	struct mm_struct      *mm;
	void                  *kernel_addr;
	unsigned long flags;
	PR_DEBUG("start");
	switch (cmd) {
	/*
	 *      Exploring VMA issues
	 */
	case 0:
		ptr = (void *)arg;
		PR_DEBUG("ptr is %p", ptr);
		vma = find_vma(current->mm, arg);
		PR_DEBUG("vma is %p", vma);
		diff = arg - vma->vm_start;
		PR_DEBUG("diff is %d", diff);
		private = (unsigned long)vma->vm_private_data;
		PR_DEBUG("private (ul) is %lu", private);
		PR_DEBUG("private (p) is %p", (void *)private);
		adjusted = private + diff;
		PR_DEBUG("adjusted (ul) is %lu", adjusted);
		PR_DEBUG("adjusted (p) is %p", (void *)adjusted);
		break;

	/*
	 *      This is asking the kernel to read the memory
	 */
	case 1:
		PR_DEBUG("starting to read");
		memcpy(str, vaddr, 256);
		str[255] = '\0';
		PR_DEBUG("data is %s", str);
		break;

	/*
	 *      This is asking the kernel to write the memory
	 */
	case 2:
		PR_DEBUG("starting to write");
		memset(vaddr, arg, size);
		break;

	/*
	 *      This demos how to take the user space pointer and turn it
	 *      into a kernel space pointer
	 */
	case 3:
		PR_DEBUG("starting to write using us pointer");
		ptr = (void *)arg;
		PR_DEBUG("ptr is %p", ptr);
		break;

	/*
	 *      mmap a region
	 */
	case 4:
		PR_DEBUG("trying to mmap");
		if (do_kmalloc) {
			kaddr = kmalloc(ioctl_size, GFP_KERNEL);
		} else {
			order = get_order(ioctl_size);
			kaddr = (void *)__get_free_pages(GFP_KERNEL, order);
		}
		mm = current->mm;
		flags = MAP_POPULATE | MAP_SHARED | MAP_LOCKED;
		flags &= ~(MAP_EXECUTABLE | MAP_DENYWRITE);
		down_write(&mm->mmap_sem);
		addr = do_mmap_pgoff(
		        filp,                                                                                                                                                                           /* file pointer */
		        (unsigned long)kaddr,                                                                                                                                                           /* address - this is the buffer we kmalloc'ed */
		        ioctl_size,                                                                                                                                                                     /* size */
		        PROT_READ | PROT_WRITE,                                                                                                                                                         /* protection */
		        flags,                                                                                                                                                                          /* flags */
		        0                                                                                                                                                                               /* pg offset */
		        );
		up_write(&mm->mmap_sem);
		PR_DEBUG("kaddr is (p) %p", kaddr);
		PR_DEBUG("real size is (d) %d", ioctl_size);
		PR_DEBUG("addr for user space is (lu) %lu / (p) %p", addr, (void *)addr);
		return(addr);

		break;

	/*
	 *      unmap a region
	 */
	case 5:
		PR_DEBUG("trying to unmap");
		vma = find_vma(current->mm, addr);
		kernel_addr = vma->vm_private_data;
		size = vma->vm_end - vma->vm_start;
		PR_DEBUG("deduced kernel_addr is %p", kernel_addr);
		PR_DEBUG("deduced size is (d) %d", size);
		PR_DEBUG("real size is (d) %d", ioctl_size);
		PR_DEBUG("real kaddr is (p) %p", kaddr);
		ret = do_munmap(current->mm, addr, ioctl_size);
		if (do_kmalloc) {
			kfree(kernel_addr);
		} else {
			order = get_order(size);
			free_pages((unsigned long)kernel_addr, order);
		}
		return(ret);

		break;

	/*
	 *      The the size of the region
	 */
	case 6:
		PR_DEBUG("setting the size");
		ioctl_size = arg;
		PR_DEBUG("size is %d", ioctl_size);
		break;
	}
	return(0);
}


/*
 * The open implementation. Currently this does nothing
 */
static int kern_open(struct inode *inode, struct file *filp) {
	PR_DEBUG("start");
	return(0);
}


/*
 * The release implementation. Currently this does nothing
 */
static int kern_release(struct inode *inode, struct file *filp) {
	PR_DEBUG("start");
	return(0);
}


/*
 * The read implementation. Currently this does nothing.
 */
static ssize_t kern_read(struct file *filp, char __user *buf, size_t count, loff_t *pos) {
	PR_DEBUG("start");
	return(0);
}


/*
 * The write implementation. Currently this does nothing.
 */
static ssize_t kern_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos) {
	PR_DEBUG("start");
	return(0);
}


/*
 *      VMA ops
 */
void kern_vma_open(struct vm_area_struct *vma) {
	PR_DEBUG("start");
}


void kern_vma_close(struct vm_area_struct *vma) {
#ifdef DO_FREE
	unsigned int order;
#endif // DO_FREE
	unsigned int size = vma->vm_end - vma->vm_start;
	void         *addr = vma->vm_private_data;
	PR_DEBUG("start");
	PR_DEBUG("pointer as long is %lu", vma->vm_start);
	PR_DEBUG("pointer as pointer is %p", (void *)(vma->vm_start));
	PR_DEBUG("addr is %p", addr);
	PR_DEBUG("size is %d", size);
#ifdef DO_FREE
	if (do_kmalloc) {
		kfree(addr);
	} else {
		order = get_order(size);
		free_pages((unsigned long)addr, order);
	}
#endif // DO_FREE
}


static struct vm_operations_struct kern_remap_vm_ops = {
	.open  = kern_vma_open,
	.close = kern_vma_close,
};

/*
 *      This is the most basic mmap implementation. It does NOT work because
 *      you don't really state WHAT memory kernel side you are mapping to user
 *      space...
 */

/*
 * static int kern_mmap_simple(struct file *filp, struct vm_area_struct *vma) {
 *      return remap_pfn_range(
 *              vma, // vma
 *              vma->vm_start, // start
 *              vma->vm_pgoff, // page number
 *              vma->vm_end-vma->vm_start, // size
 *              vma->vm_page_prot // protection
 *      );
 * }
 */

/*
 * The mmap implementation.
 */
static int kern_mmap(struct file *filp, struct vm_area_struct *vma) {
#ifdef DO_RESERVE
	// for the reserve loop
	int i;
#endif // DO_RESERVE
	// for return values
	int ret;
	// for __get_user_pages
	int order = 0;
	PR_DEBUG("start");
	size = vma->vm_end - vma->vm_start;
	PR_DEBUG("size is %d", size);

	/*
	 *      This code uses kmalloc
	 */
	if (do_kmalloc) {
		// calculate number of pages needed
		pages = size / PAGE_SIZE;
		//size_rounded=pages*PAGE_SIZE;
		if (size % PAGE_SIZE > 0) {
			pages++;
		}
		vaddr = kmalloc(size, GFP_KERNEL);
#ifdef DO_RESERVE
		// reserve the pages
		for (i = 0; i < pages * PAGE_SIZE; i += PAGE_SIZE) {
			SetPageReserved(virt_to_page(((unsigned long)vaddr) + i));
		}
#endif // DO_RESERVE
	} else {
		/*
		 *      This code used __get_free_pages
		 */
		order = get_order(size);
		addr = __get_free_pages(
		        GFP_KERNEL,
		        order
		        );
		vaddr = (void *)addr;
		PR_DEBUG("addr is %lx", addr);
		PR_DEBUG("order is %d", order);
	}
	PR_DEBUG("vaddr is %p", vaddr);
	//memset(vaddr,'a',size);
	phys = virt_to_phys(vaddr);
	pg_num = phys >> PAGE_SHIFT;
	PR_DEBUG("phys is %lx", phys);
	PR_DEBUG("pg_num is %d", pg_num);
	//PR_DEBUG("pp is %p",pp);
	//PR_DEBUG("start_addr is %p",start_addr);
	PR_DEBUG("vm_start is %lx", vma->vm_start);
	PR_DEBUG("vm_end is %lx", vma->vm_end);
	PR_DEBUG("vm_pgoff is %lx", vma->vm_pgoff);
	ret = remap_pfn_range(
	        vma,
	        vma->vm_start,
	        pg_num,
	        size,
	        vma->vm_page_prot
	        );
	if (ret) {
		PR_DEBUG("in error path");
		if (do_kmalloc) {
			kfree(vaddr);
		} else {
			free_pages((unsigned long)addr, order);
		}
		return(ret);
	}
	//memset(vaddr,'b',size);
	vma->vm_private_data = vaddr;
	vma->vm_ops = &kern_remap_vm_ops;
	kern_vma_open(vma);
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
	.unlocked_ioctl   = kern_unlocked_ioctll,
	.mmap    = kern_mmap,
//	.mmap=kern_mmap_simple,
};

int register_dev(void) {
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
	        my_class,                                                                                                                   /* our class */
	        NULL,                                                                                                                       /* device we are subdevices of */
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

void unregister_dev(void) {
	device_destroy(my_class, pdev->first_dev);
	cdev_del(&pdev->cdev);
	unregister_chrdev_region(pdev->first_dev, MINORS_COUNT);
	kfree(pdev);
	class_destroy(my_class);
}
