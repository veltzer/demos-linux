//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/fs.h> // for fops
#include <linux/device.h> // for struct device
#include <linux/eventfd.h> // for the eventfd API

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *      Driver that demos how ioctl can trigger eventfd
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// static data
static struct device* my_device;

// fops

/*
 * This is the ioctl implementation.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	// file desriptor to use
	int fd;
	// struct file to use
	struct file *fp;

	PR_DEBUG("start");
	switch (cmd) {
		case 0:
			fd = (int)arg;
			fp = eventfd_fget(fd);
			if (fp == NULL) {
				PR_DEBUG("bad file descriptor");
				return(-EINVAL);
			}
			//eventfd_signal(fp,1);
			return(0);
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
