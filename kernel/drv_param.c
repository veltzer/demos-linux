#include<linux/module.h> // for MODULE_* stuff
#include<linux/moduleparam.h> // for module_param
#include<linux/stat.h> // for the various security constants

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("demonstrate the use of kernel module parameters");

/*
 * Why would you want parameters in your module ?
 * Because like any piece of software a kernel module is complex and you
 * want to enable some kind of external control over what it does.
 * Mind you, you still have to try to make your module "just work" with
 * sensible behaviour for most users using the default values and without
 * need for any external input of parameters but:
 * - sometimes this is easier said than done.
 * - different users want to do radically different things with the
 * same module.
 *
 * How can the user change the parameters?
 * - /etc/modules may have default values for them of the form:
 * [module_name] param1=value1 param2=value2 ...
 * - at modprobe or insmod time:
 * modprobe uart ioadr=0x220
 * insmod ./mymodule.ko debug=1
 * - at run time:
 * echo 1 > /sys/modules/mymod/parameters/debug
 *
 * How can the user see the current value?
 * - through the /sys file system:
 * cat /sys/modules/[module_name]/parameters/[parameter name]
 *
 * How can a user see what parameters a compiled module has?
 * - read the documentation for that module.
 * - use modinfo:
 * modinfo [module_name] will print them.
 * - load the module (insmod or modprobe it) and look at:
 * /sys/modules/[module_name]/parameters/
 *
 * Note:
 * You should consider carefully whether or not your module can actually
 * handle changes to a parameters value at runtime. This could be extremely
 * delicate. The parameter system DOES NOT provide you with a callback
 * whenever a parameter changes. If you want that you can use the /proc
 * filesystem where you get a callback both on read and on write.
 * In any case you will probably have to use some kind of locking to protect
 * pieces of your code that implicitly assume that the value of the parameter
 * does not change while they are running. The starting point for doing this is
 * to atomically copy the parameters value before the piece of code and never
 * look at it again and only use the copy.
 */

/*
 * These are the variables that hold the values of the parameters.
 * These are just standard C variables.
 * We make them static to make sure we do not mess up the link namespace.
 * The values we give them here are the "builtin defaults".
 * Once we are in the init statement the user could have changed these values.
 * We could do some "auto detect" or "smart" code in the initialization of
 * the module but that too should ultimately be controlled by these external
 * patameters.
 * These should all have default values otherwise you should assume that they
 * are junk.
 */
static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";

/*
 * Each parameter needs to be declared using the following syntax:
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 *
 * If you want to see all the options see $KERNEL_HEADERS/linux/stat.h
 */
// this is the most permissive mode available.
// root can read write, group root can read write and others can read.
// for security reasons others writing is prohibited. This means that you
// will need root user or to be a member of group root to change kernel
// modules parameters at runtime...
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
MODULE_PARM_DESC(myshort, "A short integer");
// only root and group root will be able to both read this and write this at runtime
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
// only root will be able to read this one
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
// this will not appear at all in /sys but will appear in modinfo.
// no one has permission to do anything with it...
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

static int param_init(void) {
	INFO("start");
	INFO("myshort is a short integer: %hd", myshort);
	INFO("myint is an integer: %d", myint);
	INFO("mylong is a long integer: %ld", mylong);
	INFO("mystring is a string: %s", mystring);
	INFO("You may change some of the values now via /sys and see the values changed");
	INFO("either by catting /sys or unloading the module and looking at the unload printout...");
	return 0;
}

static void param_exit(void) {
	INFO("start");
	INFO("myshort is a short integer: %hd", myshort);
	INFO("myint is an integer: %d", myint);
	INFO("mylong is a long integer: %ld", mylong);
	INFO("mystring is a string: %s", mystring);
}

module_init(param_init);
module_exit(param_exit);
