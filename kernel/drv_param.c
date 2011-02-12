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
 */
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
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
