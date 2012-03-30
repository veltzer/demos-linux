#include<linux/module.h> // for MODULE_*
#include<linux/printk.h> // for pr_*

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("module with error in the initialisation process");

// init/exit functions
static int __init mod_init(void) {
	pr_err("mod_init");
	return(-1);
}

static void __exit mod_exit(void) {
	pr_err("mod_exit");
}

// declaration of init/cleanup functions of this module
module_init(mod_init);
module_exit(mod_exit);
