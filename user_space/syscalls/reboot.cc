#include <stdio.h>
#include <string.h>

// this is from the reboot(2) manual page...
#include <unistd.h>
#include <linux/reboot.h>

// this is from the syscall(2) manual page...

#include <unistd.h>
#include <sys/syscall.h> // For SYS_xxx definitions

#include "us_helper.hh"

/*
 *      Example of how to reboot the system
 *      You need to have the right capabilities or you have to be running as root in order
 *      to run this program. If you have the right permissions then this program will reboot
 *      your machine...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	SCIG(syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, NULL), "trying to reboot the system");
	return(0);
}
