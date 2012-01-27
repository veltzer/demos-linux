#define DEBUG
#include <linux/module.h> // for MODULE_*, module_*
#include <linux/slab.h> // for the cache functions

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("This demo is to show how to create kmem caches and use them");

/*
 * Why would you need your own cache? Or, to put it another way, is kmalloc/kfree not enough ?!?
 * Well, it depends on what you are doing. If you are doing very infrequent allocations (say on
 * init and cleanup and maybe on open and close) then you should be fine with kmalloc/kfree. If,
 * on the other hand, you are allocating lots and lots of tiny objects (imagine the network stack
 * for a second) and releasing them very frequently, and you wish to be able to debug exactly
 * how many of those you are using at any point in time, then the cache is better suited to your needs.
 * The cache also gives you the guarantee that once you populate it you are not going to be surprised
 * by allocation functions failing.
 *
 * TODO:
 * - do the allocation and deallocation from the cache via ioctl and add a user
 *	space demo application that shows slabtop(1) and /proc/slabinfo as it is allocating
 *	and deallocating.
 */

// statics for this module
static struct kmem_cache* cache_p=NULL;
static void* p=NULL;

// our own functions
static int __init kmem_init(void) {
	PR_INFO("start");
	cache_p = kmem_cache_create(
		"mark.veltzer",// name of cache (will appear in slabtop(1), /proc/slabinfo and more.
		10,// size to allocate in advance
		0,// alignment
		SLAB_HWCACHE_ALIGN | SLAB_DEBUG_OBJECTS,// flags (look at the docs, will you ?)
		NULL// ctor/dtor to be called when each element is allocated or deallocated
	);
	if (cache_p == NULL) {
		return(-ENOMEM);
	}
	p = kmem_cache_alloc(cache_p, GFP_KERNEL);
	if (p == NULL) {
		// there is not too much that we can do here
		PR_ERROR("Cannot allocate memory");
		kmem_cache_destroy(cache_p);
		return(-ENOMEM);
	}
	PR_INFO("end");
	return(0);
}

static void __exit kmem_exit(void) {
	PR_INFO("start");
	kmem_cache_free(cache_p, p);
	kmem_cache_destroy(cache_p);
	PR_INFO("end");
}

// declaration of init/cleanup functions of this module
module_init(kmem_init);
module_exit(kmem_exit);
