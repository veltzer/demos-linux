#include <linux/module.h> // for printk

#include "another_file.h" // our own API

void do_something_init(void) {
	printk(KERN_ALERT "init 2");
}

void do_something_exit(void) {
	printk(KERN_ALERT "exit 2");
}
