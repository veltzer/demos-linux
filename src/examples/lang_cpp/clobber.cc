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

/*
 * This is a demo that shows C++ allows to clobber variable names in serveral ways:
 * - inheritance.
 * - local parameters.
 */

class A {
protected:
	int a;
	int b;
};

class B : public A {
public:
	int c;
	int b;

	void doSomething(int a) {
		a=5;
		b=6;
		c=8;
		printf("a is %d\n", a);
		printf("b is %d\n", b);
		printf("c is %d\n", c);
		printf("this->a is %d\n", this->a);
		printf("this->b is %d\n", this->b);
		printf("this->c is %d\n", this->c);
	}
};

int main(int argc, char** argv, char** envp) {
	B b;
	b.doSomething(7);
	return EXIT_SUCCESS;
}
