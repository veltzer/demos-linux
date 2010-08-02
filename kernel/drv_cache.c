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
#include <linux/slab.h>

#include "kernel_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Demo module for testing");

// parameters for this module

// constants for this module

// our own functions

// This demo is to show how to create caches and use them.

struct kmem_cache *cache_p;
void              *p;

static int __init mod_init(void) {
	DEBUG("start");
	cache_p = kmem_cache_create(
	        "mark",                                                                                                                                            // name of cache
	        10,                                                                                                                                                // size to allocate
	        0,                                                                                                                                                 // alignment
	        SLAB_HWCACHE_ALIGN | SLAB_DEBUG_OBJECTS,                                                                                                           // flags
	        NULL                                                                                                                                               // ctor/dtor
	        );
	if (cache_p == NULL) {
		return(-ENOMEM);
	}
	p = kmem_cache_alloc(cache_p, GFP_KERNEL);
	if (p == NULL) {
		// there is not too much that we can do here
		DEBUG("Cannot allocate memory");
	}
	DEBUG("Allocated all");
	return(0);
}


static void __exit mod_exit(void) {
	kmem_cache_free(cache_p, p);
	//kmem_cache_free(cache_p,p);
	kmem_cache_destroy(cache_p);
	DEBUG("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
