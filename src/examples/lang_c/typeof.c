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
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for __stringify()

/*
* Demo of the typeof operator in C.
*/

int main(int argc,char** argv,char** envp) {
	/* The next one does not work */
	/*
	int __attribute__((unused)) c=6;
	printf("typeof(c) returned %s\n",__stringify(typeof(c)));
	*/

	/*
	 * Example of using typeof() to avoid writing the type yourself
	 */
	int c=6;
	/* d is the same type of c */
	typeof(c) d=c;
	printf("d is %d\n",d);

	/* e's type is a pointer to whatever c type is... */
	typeof(typeof(c) *) e=(typeof(typeof(c) *))100;
	printf("e is %p\n",e);
	return EXIT_SUCCESS;
}
