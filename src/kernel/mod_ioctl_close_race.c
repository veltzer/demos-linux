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
#include<linux/module.h> // for MODULE_*
#include<linux/fs.h> // for fops
#include<linux/device.h> // for struct device
#include<linux/delay.h> // for ssleep()

#include"shared.h" // for ioctl numbers

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("explore the race condition between ioctl and close");

// static data
static struct device* my_device;

// fops

/*
* This is the ioctl implementation.
*/
static long kern_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	PR_DEBUG("start");
	switch (cmd) {
		case IOCTL_RACE_EMPTY:
			// empty ioctl to 'just work';
			//PR_INFO("end");
			return(0);

		case IOCTL_RACE_SLEEP_SHORT:
			// long ioctl to stall for time
			msleep_interruptible(10000);
			PR_INFO("end");
			return(0);

		case IOCTL_RACE_SLEEP_LONG:
			// long ioctl to stall for time
			//ssleep(10000);
			msleep_interruptible(20000);
			PR_INFO("end");
			return(0);
	}
	PR_DEBUG("end");
	return(-EINVAL);
}

/*
* The release implementation.
*/
static int kern_release(struct inode *inode, struct file *filp) {
	PR_INFO("start");
	ssleep(2);
	PR_INFO("end");
	return(0);
}

/*
* The file operations structure.
*/
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.release=kern_release,
	.unlocked_ioctl=kern_unlocked_ioctl,
};

#include"device.inc"
