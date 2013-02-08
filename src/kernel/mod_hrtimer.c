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
#include <linux/module.h> // for MODULE_*, module_*
#include <linux/hrtimer.h> // for hrtimer_init, hrtimer_stat, hrtimer_cancel
#include <linux/ktime.h> // for ktime_set
//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo of the high resolution timer API");

/*
* This module shows how to use hi res timers in the linux kernel.
*
* References:
* http://www.ibm.com/developerworks/linux/library/l-timers-list/
*/

#define MS_TO_NS(x) (x*1E6L)

static struct hrtimer hr_timer;

static enum hrtimer_restart my_hrtimer_callback(struct hrtimer* timer) {
	printk("my_hrtimer_callback called (%ld).\n",jiffies);
	return HRTIMER_NORESTART;
}

static int __init mod_init(void) {
	ktime_t ktime;
	unsigned long delay_in_ms=200L;
	printk("HR Timer module installing\n");
	ktime=ktime_set(0,MS_TO_NS(delay_in_ms));
	hrtimer_init(&hr_timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
	hr_timer.function=&my_hrtimer_callback;
	printk("Starting timer to fire in %ldms (%ld)\n",delay_in_ms,jiffies);
	hrtimer_start(&hr_timer,ktime,HRTIMER_MODE_REL);
	return 0;
}

static void __exit mod_exit(void) {
	int ret;
	ret=hrtimer_cancel(&hr_timer);
	if (ret) printk("The timer was still in use...\n");
	printk("HR Timer module uninstalling\n");
}

module_init(mod_init);
module_exit(mod_exit);
