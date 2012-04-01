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
#include<linux/module.h> // for MODULE_*. module_*
#include<linux/printk.h> // for pr_*
#include<linux/cpumask.h> // for num_online_cpus

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

/*
 * This is a minimal module for showing the number of current online cpus
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module showing the number of online cpus");

// our own functions
static int __init mod_init(void) {
	pr_info("number of online cpus is %d",num_online_cpus());
	return(0);
}


static void __exit mod_exit(void) {
}

// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
