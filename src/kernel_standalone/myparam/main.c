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

#include <linux/module.h> // for MODULE_*, module_*
#include <linux/printk.h> // for printk and pr_* APIs
#include <linux/init.h> // for __init, __exit
#include <linux/moduleparam.h> // for module_param()

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A module showing off module parameters");

// here comes the parameters

static int myparam=1;
module_param(myparam, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
MODULE_PARM_DESC(myparam, "myparam controls bla bla bla...");

// our own functions
static int __init hello_init(void) {
	pr_info("in hello_init\n");
	pr_info("myparam is %d\n",myparam);
	return (0);
}

static void __exit hello_exit(void) {
	pr_info("myparam is %d\n",myparam);
	pr_info("in hello_exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
