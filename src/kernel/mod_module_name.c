// SPDX-License-Identifier: GPL-2.0
/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2022 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

/* #define DEBUG */
#include <linux/module.h> /* for MODULE_* stuff */
/* #define DO_DEBUG */
#include "kernel_helper.h" /* our own helper */

/*
 * This is a test to show how you can find out your own modules name
 *
 * This shows that you can access the module name in two ways:
 * - through a macro of the compiler called KBUILD_MODNAME.
 * - through access to the private data section of this module (THIS_MODULE).
 *	from this private data section you can access other info about
 *	the module (which is shown later).
 *	Actually, if you look at the source of the autogenerated c file
 *	for your module	usually called youmodule.mod.c you can see this
 *	structure used and the fact that THIS_MODULE->name is defined as
 *	KBUILD_MODNAME.
 *	You can see the entire structure at
 *	$KERNEL_HEADERS/include/linux/module.h.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");
MODULE_VERSION("1.2.3");

/* our own functions */
static int __init mod_init(void)
{
	PR_INFO("start");
	PR_INFO("name is %s", KBUILD_MODNAME);
	PR_INFO("name is %s", THIS_MODULE->name);
	PR_INFO("srcversion is %s", THIS_MODULE->srcversion);
	PR_INFO("version is %s", THIS_MODULE->version);
	/*
	 * PR_INFO("init_size is %d", THIS_MODULE->init_size);
	 * PR_INFO("core_size is %d", THIS_MODULE->core_size);
	 * PR_INFO("init_text_size is %d", THIS_MODULE->init_text_size);
	 * PR_INFO("core_text_size is %d", THIS_MODULE->core_text_size);
	 */
	PR_INFO("printk is %p", printk);
	/*
	 * using this symbol_get function you can also get kernel symbols,
	 * other modules symbols or your own exported symbols.
	 */
	PR_INFO("printk (via get) is %p", symbol_get("printk"));
	return 0;
}

static void __exit mod_exit(void)
{
	PR_INFO("start");
	PR_INFO("end");
}

/* declaration of init/cleanup functions of this module */
module_init(mod_init);
module_exit(mod_exit);
