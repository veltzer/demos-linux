//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/fs.h> // for fops
#include <linux/device.h> // for struct device
#include <linux/spinlock.h> // for the spin lock API
#include <linux/slab.h> // for the kmalloc API

//#define DO_DEBUG
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

// static data
static struct device* my_device;
DEFINE_SPINLOCK(mr_lock); // unlocked by default...
static spinlock_t *lock_t;

// fops

/*
 * This is the ioctl implementation.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	unsigned long flags;

	PR_DEBUG("start");
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
		return 0;

	case 1:
		lock_t = kmalloc(sizeof(spinlock_t), GFP_KERNEL);
		spin_lock_init(lock_t);
		spin_lock(lock_t);
		spin_unlock(lock_t);
		kfree(lock_t);
		return 0;
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

#include "device.inc"
