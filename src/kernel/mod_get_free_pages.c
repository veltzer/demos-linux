//#define DEBUG
#include<linux/module.h> // for MODULE_*
#include<linux/slab.h> // for memory allocation API

//#define DO_DEBUG
#include"kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("This driver shows how to use get_free_pages");

// This demo is to show how to get a big size buffer with __get_free_pages
// and release them...
static int __init mod_init(void) {
	int order;
	unsigned long addr;
	int i;
	/* lets allocate the pages */
	const int size = 10000;

	order = get_order(size);
	addr = __get_free_pages(
		GFP_KERNEL,
		order
	);
	/* lets manipulate the memory */
	for (i = 0; i < size; i++) {
		((char *)addr)[i] = 0;
	}
	/* lets release the memory */
	free_pages(addr, order);
	return(0);
}


static void __exit mod_exit(void) {
	PR_DEBUG("start");
}


// declaration of init/cleanup functions of this module

module_init(mod_init);
module_exit(mod_exit);
