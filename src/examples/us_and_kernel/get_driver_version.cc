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
#include <stdio.h>	// for printf(3), snprintf(3)
#include <sys/utsname.h>// for uname(2)
#include <dlfcn.h>	// for dlopen(3), dlclose(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_NULL()

/*
 * This example shows how to extract the version number from a "ko" (kernel object)
 * file...
 *
 * BEWARE: This code DOES NOT WORK!!!
 *
 * This code has led me to the conclusion that modinfo does not use dlopen to get
 * the information from the .ko files but rather mmaps them directly and uses
 * 'man 5 elf' to find the format of the executable file. Do NOT use this code - it
 * will crash your program...
 *
 * How to fix this ?
 * Get your hands on the source code of 'modinfo' and copy the relevant parts here.
 * (publish this in a blog for the interested parties...)
 *
 * EXTRA_LINK_FLAGS=-ldl
 */

void get_driver_version(const char *filename) {
	void* handle=CHECK_NOT_NULL(dlopen(filename, RTLD_LAZY));
	CHECK_NOT_M1(dlclose(handle));
}

int main(int argc, char** argv, char** envp) {
	struct utsname buf;
	CHECK_NOT_M1(uname(&buf));
	printf("os verions is %s\n", buf.release);
	char filename[256];
	snprintf(filename, 256, "/lib/modules/%s/kernel/drivers/ata/ahci.ko", buf.release);
	printf("filename is %s\n", filename);
	get_driver_version(filename);
	return EXIT_SUCCESS;
}
