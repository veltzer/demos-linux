//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for struct device
#include<linux/completion.h> // for the completion functions

//define DO_DEBUG
#include"kernel_helper.h" // our own helper

#include"shared.h" // for the ioctl numbers

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Show the completion API for completing requests from the kernel");

/*
 * The completion API is a fairly recent API within the kernel that is lighter than
 * the wait_queue API in that on each completion event you can have just one process running.
 * Meaning that the completion structure is NOT a linked list but rather just one process(thread)
 * waiting. This is good for writing various designs where we lower the request into the kernel and
 * are not sure who exactly is going to handle it at lower levels and therefore we would not like to
 * commit to specific wait queue at this time (think packet handling etc...).
 *
 * TODO:
 * - do dynamic allocation of chrdev and remove the stupid paramters for this module.
 * - remove unneeded includes.
 * - find the right h file to include for the completion API (I am including way too much).
 * - do better ioctl names instead of numbers.
 */

// static data
static struct device* my_device;
static struct completion comp;

// fops

/*
 * This is the ioctl implementation.
 */
// a completion structure
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	int i;

	PR_DEBUG("start");
	switch (cmd) {
		case IOCTL_COMPLETE_INIT:
			init_completion(&comp);
			return 0;
		case IOCTL_COMPLETE_INIT_MACRO:
			INIT_COMPLETION(comp);
			return 0;
		case IOCTL_COMPLETE_WAIT:
			wait_for_completion(&comp);
			return 0;
		case IOCTL_COMPLETE_WAIT_INTERRUPTIBLE:
			wait_for_completion_interruptible(&comp);
			return 0;
		case IOCTL_COMPLETE_WAIT_INTERRUPTIBLE_TIMEOUT:
			i = wait_for_completion_interruptible_timeout(&comp, msecs_to_jiffies(arg));
			PR_INFO("i is %d", i);
			return 0;
		case IOCTL_COMPLETE_COMPLETE:
			complete(&comp);
			return 0;
		case IOCTL_COMPLETE_COMPLETE_ALL:
			complete_all(&comp);
			return 0;
	}
	return(-EINVAL);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner   = THIS_MODULE,
	.unlocked_ioctl   = kern_unlocked_ioctll,
};

#include"device.inc"
