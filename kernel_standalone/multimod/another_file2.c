#include <linux/kernel.h>
#include <linux/module.h>

#include "another_file.h"

void do_something_init(void) {
	printk(KERN_ALERT "init 2");
}

void do_something_exit(void) {
	printk(KERN_ALERT "exit 2");
}
