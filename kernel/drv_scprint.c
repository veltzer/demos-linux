#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <asm-generic/sections.h>
#include <linux/kgdb.h>
#include <linux/ctype.h>
#include <linux/uaccess.h>
#include <linux/nmi.h>
#include <linux/delay.h>
#include <linux/kthread.h>

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
 *
 * or
 *
 * grep sys_call_table /proc/kallsyms
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module for finding the sys call table");

// this function tries to read the sys_call_table var directly...

/*
static void read_sys_call_table(void) {
	// this does not work because the symbol "sys_call_table" is not exported...
	extern void* sys_call_table;
	printk("The address of the system call table is: 0x%p\n",sys_call_table);
}
*/

/*
static void my_func(void) {
	unsigned long **sctable=NULL;
	unsigned long ptr_start=0xc0100000; // taken from /proc/kallsysms
	//extern struct   mm_struct init_mm;
	//unsigned long ptr_start=(unsigned long)&init_mm;
	unsigned long ptr_end=ptr_start+0x1000000;
	unsigned long ptr;

	unsigned long ptr_sys_open=0xc0205d20; // taken from /proc/kallsyms
	unsigned long ptr_sys_close=0xc0205ac0; // taken from /proc/kallsyms

	//unsigned long ptr_sys_close=(unsigned long)sys_close;
	//unsigned long ptr_sys_tz=(unsigned long)sys_tz;

	//extern void (*sys_sigreturn)(void);

	for(ptr=ptr_start;ptr<ptr_end;ptr+=sizeof(void*)) {
		unsigned long *p=(unsigned long*)ptr;
		if((p[__NR_open]==ptr_sys_open) &&
			(p[__NR_close]==ptr_sys_close)) {
			sctable = (unsigned long **)p;
			printk("The address of the system call table is: 0x%p\n",&sctable[0]);
			break;
		}
	}
}
*/

/*
static void print_text(void) {
	// this does not work since _text is not exported to the symbol table...
	//printk("_text is %p",_text);
}
*/

/*
// first version of sys call table calculation...
static void func(void) {
	unsigned long* ptr=(unsigned long *)((init_mm.end_code + 4) & 0xfffffffc);
	unsigned long* sys_call_table;
	int i;
	unsigned long arr[5];
	printk (KERN_INFO "Searching for sys_call_table address...\n");
	// Lookup for the table in the data section
	while((unsigned long )ptr < (unsigned long)init_mm.end_data) {
		if (*ptr == (unsigned long)sys_close) { // The hit has happend!

			printk (KERN_INFO " -> matching detected at %p\n", ptr);
			// The pointers must point to kernel code section...
			for(i = 0; i < 4 ;i++)
			{
				arr[i]=*(ptr+i);

				arr[i]=(arr[i] >> 16) & 0x0000ffff;
			}
			// And they must match each other
			if(arr[0] != arr[2] || arr[1] != arr[3])
			{
				sys_call_table=(ptr-__NR_close);
				printk (KERN_INFO"sys_call_table base found at: %p\n",sys_call_table);
				break;
			}

		}
		ptr++; // The next one...
	}

	// Saving pointers to original system call handlers.
	//original_sys_write	= sys_call_table[__NR_write];
	//original_sys_open	= sys_call_table[__NR_open];
	//original_sys_close	= sys_call_table[__NR_close];

	printk (KERN_INFO"Starting patching!\n");

	// Remapping write, open and close syscall entries syscall table with to our
	// functions.
	//sys_call_table[__NR_write]	= fake_sys_write;
	//sys_call_table[__NR_open]	= fake_sys_open;
	//sys_call_table[__NR_close]	= fake_sys_close;
}
*/

/*
static void func2(void) {
	unsigned long **sctable=NULL;
	unsigned long ptr;
	int z;
	unsigned long start_ptr=(unsigned long)&loops_per_jiffy;
	unsigned long end_ptr=(unsigned long)&boot_cpu_data;
	printk("Module is loaded!\n");
	for(ptr=start_ptr;ptr<end_ptr;ptr+=sizeof(void*)) {
		unsigned long *p=(unsigned long*)ptr;
		if (p[__NR_close] == (unsigned long) sys_close){
			sctable = (unsigned long **)p;
			printk("The address of the system call table is: 0x%p\n",&sctable[0]);
			for(z=0;z<256;z++) //this max number of the system calls should be set
			printk("The address of %d system call is 0x%p\n",z, sctable[z]);
			break;
		}
	}
}
*/

static int (*old_pipe)(int*);

static int new_pipe(int* des) {
	printk("pipe was called...\n");
	return old_pipe(des);
}

static unsigned long* get_sys_call_table(void) {
	// the next address is taken from /proc/kallsyms
	// using "cat /proc/kallsyms | grep sys_call_table"
	unsigned long* ret=(unsigned long*)0xc0592150;
	return ret;
}

static void syscall_replace(int nr,unsigned long f1,unsigned long* f2) {
	unsigned long* syscalltab=get_sys_call_table();
	*f2=syscalltab[nr];
	syscalltab[nr]=f1;
}
static void syscall_return(int nr,unsigned long f1,unsigned long* f2) {
	unsigned long* syscalltab=get_sys_call_table();
	syscalltab[nr]=*f2;
}

static int __init mod_init(void) {
	printk("Module is loading!\n");
	//func();
	//func2();
	//print_text();
	//my_func();
	//read_sys_call_table();
	syscall_replace(__NR_pipe,(unsigned long)new_pipe,(unsigned long*)&old_pipe);
	printk("Module has loaded!\n");
	return 0;
}

static void __exit mod_exit(void) {
	printk("Module is starting to unload!\n");
	syscall_return(__NR_pipe,(unsigned long)new_pipe,(unsigned long*)&old_pipe);
	printk("Module is unloaded!\n");
}
module_init(mod_init);
module_exit(mod_exit);
