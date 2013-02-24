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
#include <iostream> // for std::cout, std::endl
#include <stdlib.h> // for EXIT_SUCCESS, getenv(3)
#include <string.h> // for strcmp(3)

/*
* A simple example for using environment variables...
* This example also shows how to receive environment variables
* via the "char** envp" argument to main and how to print it.
* It also shows how to access all environment variables via
* the 'environ' extern variable.
*/

int main(int argc,char** argv,char** envp) {
	// print out the entire environment
	while(*envp!=NULL) {
		std::cout << *envp << std::endl;
		envp++;
	}
	// print it again via environ
	extern char **environ;
	char** environp=environ;
	while(*environp!=NULL) {
		std::cout << *environp << std::endl;
		environp++;
	}
	const char* varname="WHOWANTSSOUP";
	char* ptr=getenv(varname);
	if(ptr==NULL) {
		std::cout << "Please stand in line (define environment variable WHOWANTSSOUP)" << std::endl;
		std::cout << "in bash use [export WHOWANTSSOUP=kramer]" << std::endl;
	} else {
		if(strcmp(ptr,"kramer")==0) {
			std::cout << "Jumbalaya!" << std::endl;
		} else {
			if(strcmp(ptr,"newman")==0) {
				std::cout << "No soup for you!" << std::endl;
			} else {
				std::cout << "You are not kramer or newman" << std::endl;
				std::cout << "in bash use [export WHOWANTSSOUP=kramer|newman]" << std::endl;
			}
		}
	}
	return EXIT_SUCCESS;
}
