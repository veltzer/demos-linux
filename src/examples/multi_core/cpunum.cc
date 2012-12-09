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

#include <firstinclude.h>
#include <stdio.h> // for printf(3)
#include <unistd.h> // for sysconf(3)
#include <sys/sysinfo.h> // for get_nprocs_conf(3), get_nprocs(3)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This is a simple example showing how to get the number of current cpus...
* _SC_NPROCESSORS_CONF - means configured CPUs, not neccessarily working
* _SC_NPROCESSORS_ONLN - means online CPUs. You may not have access to all of
* them.
* Remmember that the number of online CPUs can change at any minute.
*/

int main(int argc,char** argv,char** envp) {
	int ncpus;
	CHECK_NOT_M1(ncpus=sysconf(_SC_NPROCESSORS_ONLN));
	printf("sysconf(_SC_NPROCESSORS_ONLN)=%d\n",ncpus);
	CHECK_NOT_M1(ncpus=sysconf(_SC_NPROCESSORS_CONF));
	printf("sysconf(_SC_NPROCESSORS_CONF)=%d\n",ncpus);
	printf("get_nprocs_conf()=%d\n",get_nprocs_conf());
	printf("get_nprocs()=%d\n",get_nprocs());
	return EXIT_SUCCESS;
}
