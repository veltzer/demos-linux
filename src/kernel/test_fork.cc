/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2015 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for close(2), fork(2), lseek(2)
#include <sys/types.h>	// for open(2), lseek(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <err_utils.h>	// for CHECK_NOT_M1()
#include <multiproc_utils.h>	// for my_system()
#include <sys/param.h>	// for MAXPATHLEN

/*
 * This application simply tests what happens at the driver level when user space
 * calls the fork(2) system call.
 */

int main(int argc, char** argv, char** envp) {
	const char* name="mod_fork";
	char filename[MAXPATHLEN];
	snprintf(filename, MAXPATHLEN, "/dev/%s", name);
	printf("Inserting the driver...\n");
	my_system("sudo sh -c \"echo starting > /dev/kmsg\"");
	my_system("sudo sh -c \"rmmod %s 2> /dev/null; /bin/true\"", name);
	my_system("sudo insmod ./%s.ko", name);
	my_system("sudo chmod 666 /dev/%s", name);

	int fd=CHECK_NOT_M1(open(filename, O_RDWR));
	pid_t pid=CHECK_NOT_M1(fork());
	const int count=10;
	if(pid) {
		// parent
		for(int i=0;i<count;i++) {
			char c;
			CHECK_NOT_M1(read(fd,&c,1));
			sleep(1);
		}
	} else {
		// child
		for(int i=0;i<count;i++) {
			char c;
			CHECK_NOT_M1(read(fd,&c,1));
			sleep(1);
		}
	}
	sleep(2);
	CHECK_NOT_M1(close(fd));
	my_system("sudo sh -c \"rmmod %s 2> /dev/null; /bin/true\"", name);
	return EXIT_SUCCESS;
}
