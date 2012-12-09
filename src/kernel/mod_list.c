/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/fs.h> // for fops
#include <linux/device.h> // for struct device
//#define DO_DEBUG
#include "kernel_helper.h" // our own helper
#include "shared.h"

/*
* Kernel module that demostrates manipulation list structures in the kernel
*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Kernel module that demostrates manipulation of list structures using kernel built in lists");

#include "list.inc"

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
			lptr=capi_list_create();
			return(0);
		case IOCTL_LIST_DESTROY:
			capi_list_destroy(lptr);
			lptr=NULL;
			return(0);
		case IOCTL_LIST_ISEMPTY:
			res=capi_list_isempty(lptr);
			PR_DEBUG("res is %d", res);
			return(0);
		case IOCTL_LIST_ADD:
			capi_list_add(lptr, (void *)arg);
			return(0);
		case IOCTL_LIST_DEL:
			p=capi_list_del(lptr);
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

#include "device.inc"
