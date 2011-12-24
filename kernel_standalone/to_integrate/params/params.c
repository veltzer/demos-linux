#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A module to demo how to use parameters");

static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";

/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

int output (const char *fmt, ...)
{
	va_list args;
	char buf[512] = KERN_ALERT;

	va_start(args, fmt);
	vsnprintf(&buf[3], 509, fmt, args);
	va_end(args);

	return printk (buf);
}

static int param_init(void)
{
	printk("Param module starting\n");
	output("myshort is a short integer: %hd\n", myshort);
	output("myint is an integer: %d\n", myint);
	output("mylong is a long integer: %ld\n", mylong);
	output("mystring is a string: %s\n", mystring);
	return 0;
}

static void param_exit(void)
{
	printk("Goodbye World!\n");
}

module_init(param_init);
module_exit(param_exit);
