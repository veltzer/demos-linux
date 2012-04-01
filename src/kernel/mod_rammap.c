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
#include<linux/module.h> // for MODULE_*
#include<linux/page-flags.h> // for PG_* constants
#include<linux/mm.h> // for page_* functions

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module which prints out the ram map");

// static data
static unsigned int physaddr = 0x32000000;
static unsigned int size = 170 * 1024 * 1024;
static void* logical;

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
