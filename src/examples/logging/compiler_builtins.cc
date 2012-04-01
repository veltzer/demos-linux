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

#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example explores compiler builtins like:
 * __FILE__, __LINE__, __func__, __FUNCTION__, __PRETTY_FUNCTION__
 * Which can be used in order to produce better error/logging/debug/warning messages.
 *
 * Notice that if you put the prints, even in an inline function, then you get the data
 * for the ** inline function ** which is the calee and not for the caller. This is
 * probably not what you want. A way to solve this is to use a macro (also demonstrated
 * below).
 *
 * Notice that __FILE__ and __LINE__ are different from __func__, __FUNCTION__ and
 * __PRETTY_FUNCTION__ in that the former are supplied by the pre processor and the latter
 * are supplied by the compiler. This distinction however is not very important.
 *
 * A frequently asked question here is how to indent the printing to match the level
 * of stack at which the printing occurs. This is not very easy as there is no variable
 * "stacklevel" or some such which indicated the stack level and can facilitate this.
 * However, you can use function entry and exit handlers, either explicit or implicit,
 * to facilitate this.
 *
 * TODO:
 */

inline void print_builtins() {
	printf("__BASE_FILE__ is %s\n",__BASE_FILE__);
	printf("__FILE__ is %s\n",__FILE__);
	printf("__LINE__ is %d\n",__LINE__);
	printf("__func__ is %s\n",__func__);
	printf("__FUNCTION__ is %s\n",__FUNCTION__);
	printf("__PRETTY_FUNCTION__ is %s\n",__PRETTY_FUNCTION__);
}
// even this will NOT cause the printing to be of the caller...
void print_builtins() __attribute__((always_inline));
void print_builtins() __attribute__((flatten));
//void print_builtins() __attribute__((artificial));

#define PRINT_BUILTINS() \
	printf("__BASE_FILE__ is %s\n",__BASE_FILE__);\
	printf("__FILE__ is %s\n",__FILE__);\
	printf("__LINE__ is %d\n",__LINE__);\
	printf("__func__ is %s\n",__func__);\
	printf("__FUNCTION__ is %s\n",__FUNCTION__);\
	printf("__PRETTY_FUNCTION__ is %s\n",__PRETTY_FUNCTION__);

class A {
	public:
		void thisMethod(int a, int b) {
			PRINT_BUILTINS();
		}
};

int main(int argc, char **argv, char **envp) {
	printf("Here is a print using the macro:\n");
	PRINT_BUILTINS();
	printf("Here is a print using the inline function:\n");
	print_builtins();
	A a;
	a.thisMethod(5,6);
	return EXIT_SUCCESS;
}
