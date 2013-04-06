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

/*
* An irq catcher module
*/
#include <linux/module.h>		/* for MODULE_* */
#include <linux/moduleparam.h>		/* for pr_info() */
#include <linux/kernel.h>		/* for printk() */
#include <linux/interrupt.h>		/* for request_irq(), free_irq() */
#include <linux/proc_fs.h>		/* for create_proc_entry(),
						remove_proc_entry() */

MODULE_AUTHOR("Mark Veltzer");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An irq catcher module");

/*
* TODO:
*/

/* the /proc entry with which you can view the number of irqs intercepted */
static const char *proc_filename = "driver/irqcatch";
/* usb2 irq number on my desktop... */
static unsigned int irq_num=23;
module_param(irq_num, uint, S_IRWXU);
/* the number if irqs got (initialised to 0 by default) */
static unsigned int counter;
/* the proc entry */
static struct proc_dir_entry* irqcatch_proc_file;

/*
* proc file callback
*/
int irqcatch_proc_reader(char *page, char **start, off_t off, int count,
		int *eof, void *data)
{
	int len = 0;
	len += sprintf(page+len, "counter=%d\n", counter);
	if (len <= off+count)
		*eof = 1;
	*start = page+off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

/*
* Interrupt callback
* We say that it was our interrupt but really shouldn't.
*/
irqreturn_t irqcatch_int_handler(int irq, void *dev)
{
	/* increase the counter. No need for atomics here
	 * since we are guaranteed by the kernel that this
	 * function will not be run concurrently... */
	counter++;
	/* this wasn't our interrupt...:) */
	return IRQ_NONE;
}

/*
* Module housekeeping.
*/
static int irqcatch_init(void)
{
	int ret;
	irqcatch_proc_file = create_proc_entry(proc_filename, 0, NULL);
	if (IS_ERR(irqcatch_proc_file)) {
		ret=PTR_ERR(irqcatch_proc_file);
		goto error_start;
	}
	irqcatch_proc_file->read_proc = irqcatch_proc_reader;

	ret = request_irq(irq_num, irqcatch_int_handler, IRQF_SHARED,
			THIS_MODULE->name, &irqcatch_proc_file);
	if (ret)
		goto error_after_proc;

	pr_info("irqcatch loaded sucessfuly\n");
	return 0;

/*
error_after_irq:
	free_irq(irq_num, &irqcatch_proc_file);
*/
error_after_proc:
	remove_proc_entry(proc_filename, NULL);
error_start:
	return ret;
}

static void irqcatch_cleanup(void)
{
	free_irq(irq_num, &irqcatch_proc_file);
	remove_proc_entry(proc_filename, NULL);
	pr_info("irqcatch unloaded succefully\n");
}

module_init(irqcatch_init);
module_exit(irqcatch_cleanup);
