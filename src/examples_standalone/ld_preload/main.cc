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
#include <stdio.h> // for fprintf(3), printf(3)
#include <math.h> // for sin(3)
#include <stdlib.h> // for atof(3), EXIT_SUCCESS

/*
* The function runs the sin function on data from the outside world because
* otherwise the wiseass compiler would computer the sin function at compiler
* time!
*/

int main(int argc,char** argv,char** envp) {
	if(argc!=2) {
		fprintf(stderr,"usage: main [x]\n");
		return -1;
	}
	double val=atof(argv[1]);
	printf("sin(%lf) is %lf\n",val,sin(val));
	return EXIT_SUCCESS;
}
