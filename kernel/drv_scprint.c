#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>

/*
 * This driver shows how to find the system call table.
 * Stolen and modified from:
 * http://downloads.securityfocus.com/downloads/scprint.tar.gz
 *
 * also see:
 * http://kerneltrap.org/node/5793
 *
 * If you want to check that this module works then use this from the command line:
 * grep syscall_call /proc/kallsyms
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module for finding the sys call table");

static int __init mod_init(void) {
	unsigned long **sctable=NULL;
	unsigned long ptr;
	int z;
	for (ptr = (unsigned long)&loops_per_jiffy;
		ptr < (unsigned long)&boot_cpu_data; ptr += sizeof(void *)) {
			unsigned long *p=(unsigned long*)ptr;
			if (p[__NR_close] == (unsigned long) sys_close){
				sctable = (unsigned long **)p;
				printk("The address of the system call table is: 0x%p\n",&sctable[0]);
				for(z=0;z<256;z++) //this max number of the system calls should be set
				printk("The address of %d system call is 0x%p\n",z, sctable[z]);
				break;
			}
		}
	return 0;
}

static void __exit mod_exit(void) {
	printk("Module is unloaded!\n");
}
module_init(mod_init);
module_exit(mod_exit);
