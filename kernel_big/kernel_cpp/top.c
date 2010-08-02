#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module written in C++");

int cpp_init(void);
void cpp_exit(void);

static int __init link_init(void) {
	return(cpp_init());
}


static void __exit link_exit(void) {
	cpp_exit();
}


void myprintk(const char *msg) {
	printk(msg);
}


void *mymalloc(unsigned int size) {
	return(kmalloc(size, GFP_KERNEL));
}


void myfree(void *pointer) {
	kfree(pointer);
}


module_init(link_init);
module_exit(link_exit);
