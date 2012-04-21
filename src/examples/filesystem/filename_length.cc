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
#include<limits.h> // for PATH_MAX, _POSIX_PATH_MAX
#include<stdlib.h> // for EXIT_SUCCESS
#include<unistd.h> // for pathconf(3), _PC_PATH_MAX
#include<us_helper.h> // for CHECK_NOT_M1()

/*
* This example shows all the different ways of accessing the maximum file length in Linux.
* As you can see different APIs return different values. Actually, the limit in Linux is
* much higher that 4096 (you can create folders that have an absolute path of more than 4096
* bytes) but the macro is good enough to use in most cases.
*/

int main(int argc,char** argv,char** envp) {
	printf("PATH_MAX is %d\n",PATH_MAX);
	printf("_POSIX_PATH_MAX is %d\n",_POSIX_PATH_MAX);
	long path_max;
	CHECK_NOT_M1(path_max=pathconf("/",_PC_PATH_MAX));
	printf("pathconf(\"/\",_PC_PATH_MAX) is %ld\n",path_max);
	return EXIT_SUCCESS;
}
