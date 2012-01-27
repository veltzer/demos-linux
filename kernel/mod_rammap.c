#define DEBUG
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>

#include <asm/e820.h>

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *	This is a driver which prints out the ram map
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// parameters for this module

// constants for this module

// our own functions
static void capi_print_addressinfo(void *logical_adr) {
	struct page *page = virt_to_page(logical_adr);

	if (page == NULL) {
		PR_INFO("unable to translate address %p to page", logical_adr);
		return;
	}
	PR_INFO("address %p, page:%p flags:0x%0*lx mapping:%p mapcount:%d count:%d\n",
		logical_adr,
		page, (int)(2 * sizeof(unsigned long)),
		page->flags, page->mapping,
		page_mapcount(page), page_count(page)
	);

	PR_INFO("PG_lru is %lu", page->flags & (1 << PG_lru));
	PR_INFO("PG_private is %lu", page->flags & (1 << PG_private));
	PR_INFO("PG_locked is %lu", page->flags & (1 << PG_locked));
	// Missing in newer kernels and so is remarked...
	//PR_INFO("PG_buddy is %lu", page->flags & (1 << PG_buddy));
	PR_INFO("PG_writeback is %lu", page->flags & (1 << PG_writeback));
	PR_INFO("PG_slab is %lu", page->flags & (1 << PG_slab));
	PR_INFO("PG_swapcache is %lu", page->flags & (1 << PG_swapcache));
	PR_INFO("PG_active is %lu", page->flags & (1 << PG_active));
	PR_INFO("PG_reserved is %lu", page->flags & (1 << PG_reserved));
}


static void capi_debug_address(unsigned int phys) {
	void* logical = __va(phys);
	void* logical2 = phys_to_virt(phys);
	unsigned int phys2 = __pa(logical);

	PR_INFO("phys is %u", phys);
	PR_INFO("logical is %p", logical);
	PR_INFO("phys2 is %u", phys2);
	PR_INFO("logical2 is %p", logical2);
	capi_print_addressinfo(logical);
}


static unsigned int physaddr = 0x32000000;
static unsigned int size = 170 * 1024 * 1024;
static void* logical;

static int __init mod_init(void) {
	PR_DEBUG("start");
	capi_debug_address(physaddr);

	/*
	 * if (!request_mem_region(physaddr,size,)) {
	 *	PR_ERROR("could not get the memory");
	 *	return 1;
	 * }
	 */
	logical = ioremap(physaddr, size);
	if (logical == NULL) {
		PR_ERROR("could not ioremap");
		release_mem_region(physaddr, size);
		return(1);
	}
	PR_INFO("got logical address %p", logical);
	//memset(logical,0,size);
	//*logical=5;
	//PR_INFO("read %c",*logical);
	//logical=phys_to_virt(physaddr);
	//for(i=0;i<170*1024*1024;i++) {
	//	logical[i]=0;
	//}
	//capi_print_addressinfo((void*)(1024*1024*700));
	//capi_print_addressinfo((void*)(1024*1024*695));
	//capi_print_addressinfo((void*)(1024*1024*720));
	return(0);
}


static void __exit mod_exit(void) {
	PR_DEBUG("start");
	iounmap(logical);
	release_mem_region(physaddr, size);
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
