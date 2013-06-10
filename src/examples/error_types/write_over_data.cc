/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example of writing data over the data segment.
 */

void do_something_with_an_integer(int val) {
	printf("val is %d\n", val);
}

int* do_something_with_intp(int* val) {
	return val+7-7;
}
int t=1945;
const volatile int a=6;

int main(int argc, char** argv, char** envp) {
	int* p=&t;
	p++;
	*p=7;
	printf("a is %d\n", a);
	printf("&a is %p\n", &a);
	printf("&t is %p\n", &t);
	printf("a is %d\n", a);
	/*
	 * // the next line will produce a compile time error...
	 * //a=5;
	 * int* p=(int*)&a;
	 * *p=7;
	 * __sync_synchronize();
	 * printf("*p is %d\n",*p);
	 * printf("*&a is %d\n",*&a);
	 * int* foo=(int*)&a;
	 * foo=do_something_with_intp(foo);
	 * printf("*foo is %d\n",*foo);
	 * do_something_with_an_integer(a);
	 */
	return EXIT_SUCCESS;
}
