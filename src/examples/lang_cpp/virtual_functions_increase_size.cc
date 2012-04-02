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
#include<iostream> // for std::cout, std::endl
#include<assert.h> // for assert(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows that a single virtual function in an object increases it's size.
 */

// A1 and A2 are exactly the same except the virtual in A2

class A1 {
	public:
		int a;

		A1(int val) {
			a=val;
		}
		void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from A, i is " << i << std::endl;
			}
		}
};

class A2 {
	public:
		int a;

		A2(int val) {
			a=val;
		}

		virtual void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from A, i is " << i << std::endl;
			}
		}
};

int main(int argc,char** argv,char** envp) {
	std::cout << "sizeof(A1) is " << sizeof(A1) << std::endl;
	std::cout << "sizeof(A2) is " << sizeof(A2) << std::endl;
	A2 obj1(4),obj2(5);
	int* p1=(int*)&obj1;
	int* p2=(int*)&obj2;
	assert(*p1==*p2);
	assert(4==*(p1+1));
	assert(5==*(p2+1));
	return EXIT_SUCCESS;
}
