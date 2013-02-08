/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <linux/device.h> // for dev_[warn|info|..]
#include <linux/fs.h> // for fops
//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
*	This is a driver which prints stuff at open and release and allows you
*	to explore how open and close in user space translate to open and release
*	calls in the kernel
*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module with open and release");

// static data
static struct device* my_device;

// fops

/*
* The open implementation. Currently this does nothing
*/
static int kern_open(struct inode *inode, struct file *filp) {
	dev_info(my_device,"start");
	return(0);
}

/*
* The release implementation. Currently this does nothing
*/
static int kern_release(struct inode *inode, struct file *filp) {
	dev_info(my_device,"start");
	return(0);
}

/*
* The file operations structure.
*/
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.open=kern_open,
	.release=kern_release,
};

#include "device.inc"
