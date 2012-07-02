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
#include<stdlib.h> // for EXIT_SUCCESS
#include<malloc.h> // for malloc_stats(3)

/*
* This example shows the use of the malloc_stats(3) function.
* In order to make it interesting it first does some multi threading
* work with some memory allocations to make the pools/arenas appear.
*
* TODO:
* - do multi threading and show malloc_stats
*/

int main(int argc,char** argv,char** envp) {
	malloc_stats();
	return EXIT_SUCCESS;
}
