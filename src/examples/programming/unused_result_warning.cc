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

/*
* This is a demo of how to write a function for which the user must check the
* return value for...
*
* There are many more attributes that you can attach to functions in order to
* get more checks from the compiler at compile time and get better code. Check
* 'info gcc' for more details.
*
* Note that you can attach the "unused" attribute to variables as below
* to avoid warnings about them being unused when you really don't want
* to check the return value.
*
* see the discussion in:
* http://stackoverflow.com/questions/3599160/unused-parameter-warnings-in-c-code
* http://dbp-consulting.com/tutorials/SuppressingGCCWarnings.html
*/

// this is how to use unused in a function declaration...
// this is the right place to put the __attribute__...
int add(int a, int b) __attribute__((warn_unused_result));

// now for the function definition (you cant put the __attribute__ here)...
int add(int a, int b) {
	return(a + b);
}

int main(int argc,char** argv,char** envp) {
	const int a = 5;
	const int b = 5;

	// the next line will produce a compile time error
	//add(5,6);

	// lets turn off gcc warnings locally to avoid the error...
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	add(5,6);
	#pragma GCC diagnostic pop
	// another way to ignore the requirement to use the result is to put it in a variable
	// but to mark the variable as "usused"...
	int c __attribute__((unused))=add(a,b);
	return EXIT_SUCCESS;
}
