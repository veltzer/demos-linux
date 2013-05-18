/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <unistd.h>	// for pause(2)
#include <stdio.h>	// for printf(3)
#include <us_helper.h>	// for CHECK_ASSERT()

/*
 * This example is to be used for showing:
 * pmap `pidof [this executables name]
 * or:
 * cat /proc/`pidof [this executables name]`/maps
 *
 * Notes:
 * - if you unremark the array below you will see a bigger
 * data segment for {1} and the same for {0}.
 * This is because {0} blocks are lazily created at runtime.
 */

int myarray[10000]={1};

int main(int argc, char** argv, char** envp) {
	while(true) {
		int ret=pause();
		// this is what is guaranteed by a clean exit
		// of pause(2)
		CHECK_ASSERT(ret==-1 && errno==EINTR);
		printf("got some signal...\n");
	}
	return 0;
}
