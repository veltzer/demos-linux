#include <linux/module.h>

/*
 * This small module explains how to write a simple hello, world module
 * - The MODULE_* macros which are used to put module meta data
 * into the compiled objects data segment.
 * - All the functions are static which is good since it doesn't cause
 * colisions with other modules or kernel core code. This is the way
 * you should code in the kernel. Change this only when you give
 * services to other modules or to other source files.
 * - the module_* macros register the function pointers you give them
 * as the start and end functions of your module.
 * - if you fail to register an init function then you will simply not
 * have one (usually you will not want this!).
 * - if you fail to register an exit function then your module will
 * have to be unloaded forcefully (rmmod -f) which is not a good
 * idea.
 * - the __init attribute tells the kernel runtime loader that the
 * code for the routine it is attached too is connected with the
 * modules load procedure and can be discarded once the loading is
 * done.
 * - the __exit attribute tells the kernel runtime loader that the
 * code for the routine it is attached too is connected with the
 * module unload procedue and can be discarded if unloading modules
 * is prohibited.
 * - notice that the printk messages must end with a newline so
 * that the log will actually get flushed and you will see them.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Simple hello world module");

static int __init hello_init(void)
{
	printk("Hello World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("Goodbye World!\n");
}

module_init(hello_init);
module_exit(hello_exit);
