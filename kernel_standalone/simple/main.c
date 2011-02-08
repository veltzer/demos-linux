#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");

static int hello_init(void)
{
	printk("Hello World!\n");
        return 0;
}

static void hello_exit(void)
{
	printk("Goodbye World!\n");
}

module_init(hello_init);
module_exit(hello_exit);
