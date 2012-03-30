//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for struct device

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

#include"shared.h"

/*
 *      Kernel module that demostrates manipulation list structures in the kernel
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Kernel module that demostrates manipulation of list structures using kernel built in lists");

#include"list.inc"

// static data
static struct device* my_device;
static void *lptr=NULL;

// fops

/*
 * This is the ioctl implementation.
 */
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	int res;
	void *p;

	PR_DEBUG("start");
	switch (cmd) {
		case IOCTL_LIST_CREATE:
			lptr = capi_list_create();
			return(0);
		case IOCTL_LIST_DESTROY:
			capi_list_destroy(lptr);
			lptr=NULL;
			return(0);
		case IOCTL_LIST_ISEMPTY:
			res = capi_list_isempty(lptr);
			PR_DEBUG("res is %d", res);
			return(0);
		case IOCTL_LIST_ADD:
			capi_list_add(lptr, (void *)arg);
			return(0);
		case IOCTL_LIST_DEL:
			p = capi_list_del(lptr);
			PR_DEBUG("p is %d", (int)p);
			return(0);
		case IOCTL_LIST_PRINT:
			capi_list_print(lptr);
			return(0);
	}
	return(-EINVAL);
}

/*
 * The file operations structure.
 */
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.unlocked_ioctl=kern_unlocked_ioctll,
};

#include"device.inc"
