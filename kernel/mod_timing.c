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
#include <linux/timex.h>
#include <linux/cpufreq.h>
#include <asm/div64.h>
#include <linux/delay.h> // for udelay

#include "kernel_helper.h" // our own helper

/*
 *	This driver shows how various timing delays are done in the kernel
 */

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
static void long_code(unsigned long mic) {
	/*
	 * int i;
	 * for(i=0;i<10000000;i++) {
	 *	void* ptr=kmalloc(1000,GFP_KERNEL);
	 *	kfree(ptr);
	 * }
	 */
	// sleep for 1 second
	//msleep(mil);
	//ssleep(1);
	udelay(mic);
}


/*
 * This is the ioctl implementation.
 */
// cycles_t is actually unsigned long long (look at arch/x86/include/asm/tsc.h).
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	// for register measurements...
	cycles_t curreg, cnt1, cnt2;
	unsigned long cdiff, crmil, crmic, crmic2;
	// for jiffies measurements...
	unsigned long j1, j2, jdiff, jmil, jmic;
	unsigned int freq;

	PR_DEBUG("start");
	//char str[256];
	//PR_DEBUG(str,"start");
	switch (cmd) {
	case 0:
		/* this shows how to work with the x86 counters */
		curreg = get_cycles();
		PR_DEBUG("get_cycles: %llu\n", curreg);
		// getting the cpufreq for cpu0
		// I used the quick version under the assumption that the
		// frequency doesn't change. If this assumption is not
		// correct and the cpu scales for some reason you need
		// to use 'cpufreq_get'.
		freq = cpufreq_quick_get(0);
		PR_DEBUG("cpufreq_quick_get: %i\n", freq);
		break;

	case 1:
		/* this is how to measure the speed of some code using counters */
		freq = cpufreq_quick_get(0);
		cnt1 = get_cycles();
		long_code(arg);
		cnt2 = get_cycles();
		cdiff = cnt2 - cnt1;
		crmil = cdiff / freq;
		crmic = crmil * 1000;
		crmic2 = (cdiff * 1000) / freq;
		PR_DEBUG("cnt1: %llu\n", cnt1);
		PR_DEBUG("cnt2: %llu\n", cnt2);
		PR_DEBUG("cdiff: %lu\n", cdiff);
		PR_DEBUG("crmil: %lu\n", crmil);
		PR_DEBUG("crmic: %lu\n", crmic);
		PR_DEBUG("crmic2: %lu\n", crmic2);
		break;

	case 2:

		/*
		 *	This shows how to work with jiffies...
		 *	It will demostrate the most important attribute of jiffies and that
		 *	is that juffies DO NOT change
		 */
		j1 = jiffies;
		long_code(arg);
		j2 = jiffies;
		jdiff = (j2 - j1) * 1000;
		//jmic=do_div(jdiff,HZ);
		jmil = jdiff / HZ;
		jmic = jmil * 1000;
		PR_DEBUG("j1 is %lu", j1);
		PR_DEBUG("j2 is %lu", j2);
		PR_DEBUG("jdiff is %lu", jdiff);
		PR_DEBUG("HZ is %d", HZ);
		PR_DEBUG("jmil is %lu", jmil);
		PR_DEBUG("jmic is %lu", jmic);
		break;

	case 3:

		/*
		 *	This syscall does nothing on purpose to enable timing
		 *	from user space
		 */
		//udelay(24);
		break;
	}
	return(0);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_unlocked_ioctll,
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
