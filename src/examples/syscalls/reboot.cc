/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <unistd.h>	// for reboot(2), syscall(2)
#include <linux/reboot.h>	// for reboot(2)
#include <sys/syscall.h>// for syscall(2), SYS_xxx definitions
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * Example of how to reboot the system
 * You need to have the right capabilities or you have to be running as root in order
 * to run this program. If you have the right permissions then this program will reboot
 * your machine, so you better watch out...
 */

int main(int argc, char** argv, char** envp) {
	CHECK_NOT_M1(syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, NULL));
	return EXIT_SUCCESS;
}
