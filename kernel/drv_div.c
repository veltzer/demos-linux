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

#include "kernel_helper.h"

#include "div_buffer.h"

/*
 *      Driver that demostrates arithmetic operations in the kernel...
This demo is all about doing arithmetic in the kernel.
======================================================
- If you compile on kernel 2.6.31-14-generic (ubuntu 9.10)
- If you compile on kernel 2.6.28-15-generic (ubuntu 9.04)
	you will find that the symbol __udivdi3 is missing.
	You see that in two place:
	- when you compile the module you get the warning:
	WARNING: "__udivdi3" [/home/mark/bla/kernel_arithmetic/demo.ko] undefined!
	- when you try to insmod the module you get the error:
	[12697.177574] demo: Unknown symbol __udivdi3
	This means that you need to link with libgcc.
	just run 'make relink'.
 */

// parameters for this module

static int chrdev_alloc_dynamic = 1;
static int first_minor = 0;
static int kern_major = 253;
static int kern_minor = 0;
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

/*
 * This is the ioctl implementation. Currently this function supports
 * getting the image rows and columns
 */
static int kern_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	// the buffer which will be used for the transaction
	buffer b;

	DEBUG("start");
	switch (cmd) {
	case 0:
		// get the data from the user
		if (copy_from_user(&b, (void *)arg, sizeof(b))) {
			ERROR("problem with copy_from_user");
			return(-EFAULT);
		}
		DEBUG("after copy");
		INFO("b.u1 is %llu", b.u1);
		INFO("b.u2 is %llu", b.u2);
		INFO("b.d1 is %lld", b.d1);
		INFO("b.d2 is %lld", b.d2);
		b.udiv = b.u1 / b.u2;
		b.umul = b.u1 * b.u2;
		b.uadd = b.u1 + b.u2;
		b.usub = b.u1 - b.u2;
		b.ddiv = b.d1 / b.d2;
		b.dmul = b.d1 * b.d2;
		b.dadd = b.d1 + b.d2;
		b.dsub = b.d1 - b.d2;
		// copy the data back to the user
		if (copy_to_user((void *)arg, &b, sizeof(b))) {
			ERROR("problem with copy_to_user");
			return(-EFAULT);
		}
		// everything is ok
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
	.ioctl = kern_ioctl,
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
	my_device = device_create(
	        my_class,                                                                                                                   /* our class */
	        NULL,                                                                                                                       /* device we are subdevices of */
	        pdev->first_dev,
	        NULL,
	        "%s",
	        name
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


module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// this is where the 64 bit division magic starts...

// and here is the division function:

/*
 * 64bit division - for sync stuff..
 */

/*
 #define udiv_qrnnd(q, r, n1, n0, d) \
 * __asm__ ("divl %4" \
 *         : "=a" ((u32)(q)), \
 *           "=d" ((u32)(r)) \
 *         : "0" ((u32)(n0)), \
 *           "1" ((u32)(n1)), \
 *           "rm" ((u32)(d)))
 *
 #define u64_div(x,y,q) do {u32 __tmp; udiv_qrnnd(q, __tmp, (x)>>32, x, y);} while (0)
 #define u64_mod(x,y,r) do {u32 __tmp; udiv_qrnnd(__tmp, q, (x)>>32, x, y);} while (0)
 #define u64_divmod(x,y,q,r) udiv_qrnnd(q, r, (x)>>32, x, y)
 */

/*
 #define _FP_W_TYPE_SIZE         32
 #define _FP_W_TYPE              unsigned int
 #define _FP_WS_TYPE             signed int
 #define _FP_I_TYPE              int
 *
 #include <math-emu/op-1.h>
 #include <math-emu/op-2.h>
 #include <math-emu/op-4.h>
 #include <math-emu/op-common.h>
 */
// there is no such file for x86
//#include <asm/sfp-machine.h>
// creates compilation issues...
//#include <math-emu/soft-fp.h>
// there is no such file for x86
//#include <math-emu/sfp-util.h>

// this is what gives us the division...
#include <asm/div64.h>

unsigned long long __udivdi3(unsigned long long divided, unsigned long long divisor) {
	unsigned int reminder;

	DEBUG("divided is %llu", divided);
	DEBUG("divisor is %llu", divisor);
	return(div_u64_rem(divided, divisor, &reminder));
}


long long __divdi3(long long divided, long long divisor) {
	unsigned int reminder;

	DEBUG("divided is %lld", divided);
	DEBUG("divisor is %lld", divisor);
	return(div_u64_rem(divided, divisor, &reminder));
}
