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
#include <zlib.h> // for compress2(3)
#include <stdio.h> // for printf(3)
#include <string.h> // for strlen(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
* A simple example of using the libz library.
*
* EXTRA_LINK_FLAGS=-lz
*/

int main(int argc,char** argv,char** envp) {
	const char *source="Merry had a little lamb";
	unsigned long sourcelen=strlen(source);
	unsigned long destlen=100;
	unsigned char *dest=new unsigned char[destlen];
	int code=compress2(dest, &destlen, (const unsigned char *)source, sourcelen, Z_DEFAULT_COMPRESSION);
	if (code!=Z_OK) {
		printf("compress failed with code [%d]\n", code);
		return(1);
	}
	printf("result is [%s]\n", dest);
	return EXIT_SUCCESS;
}
