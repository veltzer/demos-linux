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
#include <linux/pagemap.h>

#define DO_DEBUG
#include "kernel_helper.h"

/*
 * This driver shows how to find the system call table.
 *
 * references:
 * http://downloads.securityfocus.com/downloads/scprint.tar.gz
 * http://www.epanastasi.com/?page_id=52
 * http://kerneltrap.org/node/5793
 * http://stackoverflow.com/questions/2103315/linux-kernel-system-call-hooking-example
 * http://rootkitanalytics.com/kernelland/syscall-hijack.php
 *
 * If you want to check that this module works then use this from the command line:
 * grep syscall_call /proc/kallsyms
 *
 * or
 *
 * grep sys_call_table /proc/kallsyms
 *
 * TODO:
 * - make finding the sys call table be dynamic and not with the current hard code
 *   address.
 * - how come I don't manage to do this with the set_memory_{rw},{x} etc? They are
 *   supposed to work. Check it out again.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module for finding the sys call table");

// A function to align an address to a page boundary...
inline unsigned long align_address(unsigned long addr) {
	return addr & ~(PAGE_SIZE-1);
}

// this function prints system call numbers (this is good for getting an idea
// of how big the system call table really is).

/*
static void print_sys_call_nums(void) {
	DEBUG("__NR_open is %d",__NR_open);
	DEBUG("__NR_close is %d",__NR_close);
	DEBUG("__NR_pipe %d",__NR_pipe);
	DEBUG("__NR_write %d",__NR_write);
}
*/

// this functions puts whatever permission on whatever address you want
// prot should be or'ed with VM_READ | VM_WRITE | VM_EXEC

/*
static void set_perms(void* pointer,int prot_val) {
	struct page *pg;
	pgprot_t prot;
	// find the page of the address
	pg=virt_to_page(pointer);
	// setup protectction with the right value
	prot.pgprot=prot_val;
	// now change the values (is there a return value ?!?)
	// this function is deprecated in new kernels
	change_page_attr(pg,1,prot);
}
*/

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

static unsigned long* get_sys_call_table(void) {
	// the next address is taken from /proc/kallsyms
	// using "cat /proc/kallsyms | grep sys_call_table"
	unsigned long* ret=(unsigned long*)0xc0594150;
	return ret;
}

static void* sys_call_adr=NULL;
static unsigned long* sys_call_adr_precise=NULL;

static int map_sys_call_table(void) {
	unsigned long* syscalltab=get_sys_call_table();
	unsigned long syscalladr=(unsigned long)virt_to_phys(syscalltab);
	unsigned long offset=syscalladr & (PAGE_SIZE-1);
	unsigned long start=align_address(syscalladr);
	unsigned long len=PAGE_SIZE;
	//const unsigned int num_pages=1;
	sys_call_adr=ioremap(start, len);
	if(sys_call_adr==NULL) {
		ERROR("unable to ioremap");
		return -1;
	} else {
		sys_call_adr_precise=sys_call_adr+offset;
		DEBUG("got precise %p",sys_call_adr_precise);
		return 0;
	}
}

static void unmap_sys_call_table(void) {
	iounmap(sys_call_adr);
}

static unsigned long get_sys_call_entry(int nr) {
	return sys_call_adr_precise[nr];
}
static void set_sys_call_entry(int nr,unsigned long val) {
	sys_call_adr_precise[nr]=val;
}

static void test_sys_call_table(int nr) {
	unsigned long val=get_sys_call_entry(nr);
	DEBUG("val is %lx",val);
	set_sys_call_entry(nr,val);
}

/*
static int syscall_replace(int nr,unsigned long f1,unsigned long* f2) {
	unsigned long* syscalltab=get_sys_call_table();
	unsigned long syscalladr=(unsigned long)syscalltab;
	unsigned long start=align_address(syscalladr);
	const unsigned int num_pages=1;
	int ret;
	ret=set_memory_rw(start,num_pages);
	if(ret) {
		ERROR("unable to set_memory_rw");
		return ret;
	}
	*f2=syscalltab[nr];
	syscalltab[nr]=f1;
	ret=set_memory_ro(start,num_pages);
	if(ret) {
		ERROR("unable to set_memory_ro");
		return ret;
	}
	return 0;
}

static int syscall_return(int nr,unsigned long f1,unsigned long* f2) {
	unsigned long* syscalltab=get_sys_call_table();
	int ret;
	ret=set_memory_rw(align_address((unsigned long)syscalltab),1);
	if(ret) {
		ERROR("unable to set_memory_rw");
		return ret;
	}
	syscalltab[nr]=*f2;
	ret=set_memory_ro(align_address((unsigned long)syscalltab),1);
	if(ret) {
		ERROR("unable to set_memory_ro");
		return ret;
	}
	return 0;
}

static int (*old_pipe)(int*);
static int new_pipe(int* des) {
	printk("pipe was called...\n");
	return old_pipe(des);
}
*/
/*
static int (*old_pipe2)(int*,int);
asmlinkage static int new_pipe2(int* des,int flags) {
	DEBUG("here");
	return old_pipe2(des,flags);
}
*/
static int called=0;
static asmlinkage long (*old_close)(unsigned int);
asmlinkage static long new_close(unsigned int val) {
	DEBUG("here");
	called++;
	return old_close(val);
}

static int __init mod_init(void) {
	DEBUG("start");
	map_sys_call_table();
	test_sys_call_table(__NR_close);
	old_close=(asmlinkage long (*)(unsigned int))get_sys_call_entry(__NR_close);
	set_sys_call_entry(__NR_close,(unsigned long)new_close);
	DEBUG("end");
	return 0;
}

static void __exit mod_exit(void) {
	DEBUG("start");
	set_sys_call_entry(__NR_close,(unsigned long)old_close);
	unmap_sys_call_table();
	DEBUG("called is %d",called);
	DEBUG("end");
}
module_init(mod_init);
module_exit(mod_exit);
