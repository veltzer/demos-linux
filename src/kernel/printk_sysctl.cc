/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<firstinclude.h>
#include<stdio.h> // for printf(3), perror(3)
#include<errno.h> // for perror(3)
#include<sys/sysctl.h> // for sysctl(3)
//#include<unistd.h> // for sysctl(2)
//#include<linux/sysctl.h> // for sysctl(2)
#include<stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE

#define SIZE(a) (sizeof(a)/sizeof((a)[0]))
int name[]={CTL_KERN,KERN_PRINTK};
int printk_params[4];
int new_params[4];

/*
* This example shows how to control kernel logging level using a user space program.
* You can also use /proc or sysctl via the command line to do the same thing
*/

int main(int argc, char** argv, char** envp) {
	size_t paramlth=sizeof(printk_params);
	if(argc==1) {
		/* report */
		if (sysctl(name, SIZE(name), printk_params, &paramlth, 0, 0)) {
			perror("sysctl");
			exit(EXIT_FAILURE);
		}
		printf("got %d bytes:\n", paramlth);
		printf("console_loglevel: %d\n", printk_params[0]);
		printf("default_message_loglevel: %d\n", printk_params[1]);
		printf("minimum_console_loglevel: %d\n", printk_params[2]);
		printf("default_console_loglevel: %d\n", printk_params[3]);
	} else if(argc==5) {
		for (int i=0;i<4;i++)
			new_params[i]=atoi(argv[i+1]);
		/* set */
		if (sysctl(name, SIZE(name), 0, 0, new_params, sizeof(new_params))) {
			perror("sysctl");
			exit(EXIT_FAILURE);
		}
		printf("set new printk parameters\n");
	} else {
		fprintf(stderr, "Call: %s [N N N N]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
