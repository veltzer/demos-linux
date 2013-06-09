/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

/*
* procfs4.c - create a "file" in /proc
* This program uses the seq_file library to manage the /proc file.
*/

/* #define DEBUG */
#include <linux/module.h> /* for the MODULE_* */
#include <linux/printk.h> /* for printk and the pr_* API */
#include <linux/proc_fs.h> /* Necessary because we use proc fs */
#include <linux/seq_file.h> /* for seq_file */
/* #define DO_DEBUG */
#include "kernel_helper.h"

#define PROC_NAME KBUILD_MODNAME

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("a module showing how to use the _seq /proc API");

/*
* This function is called at the beginning of a sequence.
* ie, when:
* - the /proc file is read (first time)
* - after the function stop (end of sequence)
*
*/
static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	pr_debug("my_seq_start %p, %lld\n", s, *pos);
	/* beginning a new sequence ? */
	if (*pos == 0) {
		/* yes, return a non null value to begin the sequence */
		int *p = kmalloc(sizeof(int), GFP_KERNEL);
		*p = 0;
		return p;
	} else
		/* no, it's the end of the sequence,
		return NULL to stop reading */
		return NULL;
}

/*
* This function is called after the beginning of a sequence.
* It's called untill the return is NULL (this ends the sequence).
*/
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	int *p;
	pr_debug("my_seq_next %p, %p, %lld\n", s, v, *pos);
	p = (int *)v;
	if (*p < 10) {
		(*p)++;
		(*pos)++;
		return v;
	} else
		return NULL;
}

/*
* This function is called at the end of a sequence
*/
static void my_seq_stop(struct seq_file *s, void *v)
{
	pr_debug("my_seq_stop %p, %p\n", s, v);
	kfree(v);
}

/*
* This function is called for each "step" of a sequence
*/
static int my_seq_show(struct seq_file *s, void *v)
{
	int *p;
	pr_debug("my_seq_show %p, %p\n", s, v);
	p = (int *)v;
	seq_printf(s, "%d\n", *p);
	return 0;
}

/*
* This structure gather "function" to manage the sequence
*/
static const struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next = my_seq_next,
	.stop = my_seq_stop,
	.show = my_seq_show
};

/*
* This function is called when the /proc file is open.
*/
static int my_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &my_seq_ops);
};

/*
* This structure gathers "functions" that manage the /proc file
*/
static const struct file_operations my_file_ops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

/*
* This function is called when the module is loaded
*/
int __init init_module(void)
{
	struct proc_dir_entry *entry;
	entry = create_proc_entry(PROC_NAME, 0, NULL);
	if (IS_ERR(entry)) {
		PR_ERROR("error in create_proc_entry");
		return PTR_ERR(entry);
	}
	entry->proc_fops = &my_file_ops;
	pr_info(KBUILD_MODNAME " loaded successfully\n");
	return 0;
}

/*
* This function is called when the module is unloaded.
*/
void __exit cleanup_module(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_info(KBUILD_MODNAME " unloaded successfully\n");
}
