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
#include <iostream>	// for std::cout, std::endl
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This example investigates the sizeof() of C++ object.
 */

class A {
public:
	int a;
	char b;
	int c;
	char d;

	void dosomething() {
		for(int i=0; i<4; i++) {
			std::cout << "Hello from A, i is " << i << std::endl;
		}
	}
};

class B : public A {
public:
	int e;
	char f;
	void dosomething() {
		for(int i=0; i<4; i++) {
			std::cout << "Hello from B, i is " << i << std::endl;
		}
	}
};
#define CppOffsetOf(className, FieldName) ((char *)(&(((className *)1)->FieldName)) - (char *)1)

class C {
};

int main(int argc, char** argv, char** envp) {
	std::cout << "sizeof(A) is " << sizeof(A) << std::endl;
	std::cout << "sizeof(B) is " << sizeof(B) << std::endl;
	std::cout << "sizeof(C) is " << sizeof(C) << std::endl;
	// std::cout << "__builtin_offsetof(e,B) " << __builtin_offsetof(e,B) << std::endl;
	std::cout << "CppOffsetOf(B,e) " << CppOffsetOf(B, e) << std::endl;
	std::cout << "CppOffsetOf(A,a) " << CppOffsetOf(A, a) << std::endl;
	std::cout << "CppOffsetOf(A,b) " << CppOffsetOf(A, b) << std::endl;
	std::cout << "CppOffsetOf(A,c) " << CppOffsetOf(A, c) << std::endl;
	std::cout << "CppOffsetOf(A,d) " << CppOffsetOf(A, d) << std::endl;

	A* a=new A();
	a->dosomething();
	A* ab=new B();
	ab->dosomething();
	B* b=new B();
	b->dosomething();
	A anotherA;
	anotherA.dosomething();
	return EXIT_SUCCESS;
}
