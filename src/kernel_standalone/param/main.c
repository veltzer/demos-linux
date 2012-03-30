#include<linux/module.h>  // for MODULE_*, module_*
#include<linux/printk.h> // for printk and pr_* APIs
#include<linux/init.h> // for __init, __exit
#include<linux/moduleparam.h> // for module_param()

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A module showing off module parameters");

// here comes the parameters

static int myparam=1;
module_param(myparam, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
MODULE_PARM_DESC(myparam, "myparam controls bla bla bla...");

// our own functions
static int __init hello_init(void) {
	pr_info("in hello_init");
	pr_info("myparam is %d",myparam);
	return (0);
}

static void __exit hello_exit(void) {
	pr_info("myparam is %d",myparam);
	pr_info("in hello_exit");
}

module_init(hello_init);
module_exit(hello_exit);
