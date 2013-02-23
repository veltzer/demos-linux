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

#include "connect.h"

/* functions that we need to supply */
extern "C" {
/* support functions */
void myprintk(const char *);
void *mymalloc(unsigned int);
void myfree(void *);
}

/* to satisfy the kernel dynamic linker */
int __gxx_personality_v0;
int _Unwind_Resume;

/* support code for new and delete */
void *operator new(unsigned int x) {
	return(mymalloc(x));
}


void operator delete(void *pointer) {
	myfree(pointer);
}


class A {
public:
	int x;
	A() {
		myprintk("inside A's constructor!\n");
	}
	~A() {
		myprintk("inside A's destructor!\n");
	}
};

static A *a;

int cpp_init() {
	a=new A();

	return(0);
}

void cpp_exit() {
	delete a;
}
