/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_ASSERT()

/*
 * Example of padding a structure to L2 cache line size bytes via union...
 * Notice how to avoid the cache line size being hardcoded in your code...
 * Are there other ways of padding the structure without the union? yes.
 * This also shows that the __attribute__((aligned (LEVEL2_CACHE_LINESIZE)))
 * will cause the structure to always be aligned by the compiler when passing
 * it around and creating it on the stack.
 * Both ways DO NOT force malloc to allocate these structures aligned. You will
 * still need something like posix_memalign(3) for that...
 *
 * EXTRA_COMPILE_CMDS=echo -DLEVEL2_CACHE_LINESIZE=`getconf LEVEL2_CACHE_LINESIZE`
 * required for the static_assert below to work...
 * EXTRA_COMPILE_FLAGS=-std=c++0x
 *
 * References:
 * http://stackoverflow.com/questions/1215612/structure-padding-in-c
 *
 * TODO:
 * - the union could be made transparent using a transparent_union attribute. check
 * out the gcc documentation and put this in.
 */

struct foo {
	union _un {
		struct _pad_struct {
			char irrelevant[LEVEL2_CACHE_LINESIZE];
		} pad_struct;
		struct _real {
			int a;
			int b;
			int c;
		} real;
	} un;
};

// this guarantess that the size of the structure will be exactly 64 bytes
struct bar {
	char c1;
	int i1;
	char c2;
} __attribute__((aligned (LEVEL2_CACHE_LINESIZE)));

// same as before, applied to a Class instead...
class MyClass {
private:
	char c1;
	int i1;
	char c2;
} __attribute__((aligned (LEVEL2_CACHE_LINESIZE)));

static_assert(sizeof(foo)==LEVEL2_CACHE_LINESIZE, "size of foo is wrong");

void myfunction(char c, struct bar mybar, char l, struct foo myfoo) {
	CHECK_ASSERT((int)&mybar%LEVEL2_CACHE_LINESIZE==0);
	CHECK_ASSERT((int)&myfoo%LEVEL2_CACHE_LINESIZE!=0);
}

int main(int argc, char** argv, char** envp) {
	printf("LEVEL2_CACHE_LINESIZE=%d\n", LEVEL2_CACHE_LINESIZE);
	printf("sizeof(foo)=%d\n", sizeof(foo));
	printf("sizeof(bar)=%d\n", sizeof(bar));
	printf("sizeof(MyClass)=%d\n", sizeof(MyClass));
	struct bar mybar;
	struct foo myfoo;
	CHECK_ASSERT((int)&mybar%LEVEL2_CACHE_LINESIZE==0);
	CHECK_ASSERT((int)&myfoo%LEVEL2_CACHE_LINESIZE!=0);
	myfunction('4', mybar, '5', myfoo);
	return EXIT_SUCCESS;
}
