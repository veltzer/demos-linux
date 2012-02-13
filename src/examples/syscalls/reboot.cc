#include <unistd.h> // for reboot(2), syscall(2)
#include <linux/reboot.h> // for reboot(2)
#include <sys/syscall.h> // for syscall(2), SYS_xxx definitions

#include "us_helper.hh" // our user space helper

/*
 *      Example of how to reboot the system
 *      You need to have the right capabilities or you have to be running as root in order
 *      to run this program. If you have the right permissions then this program will reboot
 *      your machine, so you better watch out...
 *
 *              Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	CHECK_NOT_M1(syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, NULL));
	return(0);
}
