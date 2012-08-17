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
#include<linux/module.h> // for module_*, MODULE_*
#include<linux/fs.h> // for fops definitions
#include<linux/device.h> // for struct device

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

#include"shared.h" // for ioctl numbers

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Showing how to return error codes from kernel to use space");

/*
* This driver explores how to correctly return error code from kernel code
* and what happens in user space.
*/

// static data
static struct device* my_device;

// fops

/*
* This is the ioctl implementation. This returns whatever it gets.
*/
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	PR_DEBUG("start");
	return(arg);
}

/*
* The file operations structure.
*/
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.unlocked_ioctl=kern_unlocked_ioctll,
};

#include"device.inc"
