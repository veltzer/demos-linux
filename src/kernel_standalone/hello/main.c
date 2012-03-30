#include<linux/module.h>  // for MODULE_*, module_*
#include<linux/printk.h> // for printk and pr_* APIs
#include<linux/init.h> // for __init, __exit

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("minimal Hello, World! driver doing just init and cleanup");

static int __init hello_init(void) {
	pr_info("in hello_init");
	return (0);
}

static void __exit hello_exit(void) {
	pr_info("in hello_exit");
}

module_init(hello_init);
module_exit(hello_exit);
