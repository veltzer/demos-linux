/*
 *      This file is part of the linuxapi project.
 *      Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 *      The linuxapi package is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      The linuxapi package is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with the GNU C Library; if not, write to the Free
 *      Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *      02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3), fgets(3)
#include <string.h>	// for strlen(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_NULL()

int main(int argc, char** argv, char** envp) {
	const int STRINGSIZE=30;
	char mystring[STRINGSIZE];
	printf("Give me a string to check: ");
	CHECK_NOT_NULL(fgets(mystring, STRINGSIZE, stdin));
	int len=strlen(mystring);
	printf("len is %d\n", len);
	int palindrom=1;
	int i;
	for(i=0; i<len; i++) {
		if(mystring[i]!=mystring[len-i-1]) {
			palindrom=0;
			break;
		}
	}
	if(palindrom) {
		printf("YESS!!!!! string IS a palindrom...\n");
	} else {
		printf("bummer, string is not a palindrom...\n");
	}
	return EXIT_SUCCESS;
}
