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
#include<stdio.h> // for printf(3)
#include<unistd.h> // for sysconf(3)

#include<us_helper.h>

/*
 * An example showing how to get the cache line size on linux.
 * DCACHE is the data cache.
 * ICACHE is the instruction cache.
 *
 * There are many other parameters you can get via the sysconf interface. Some hardware
 * related and some software. do 'getconf -a' on the cmdline to see them...
 */
int main(int argc,char** argv,char** envp) {
	int linesize;
	CHECK_NOT_M1(linesize=sysconf(_SC_LEVEL1_DCACHE_LINESIZE));
	printf("sysconf(_SC_LEVEL1_DCACHE_LINESIZE)=%d\n",linesize);
	CHECK_NOT_M1(linesize=sysconf(_SC_LEVEL1_ICACHE_LINESIZE));
	printf("sysconf(_SC_LEVEL1_ICACHE_LINESIZE)=%d\n",linesize);
	return EXIT_SUCCESS;
}
