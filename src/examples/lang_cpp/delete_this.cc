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

#include<us_helper.h>

/*
 * This is an example of an object deallocating itself.
 * The idea is to put a "delete this" at the VERY END of some
 * method.
 * You cannot put this in the destructor since you will cause
 * an infinite regress.
 *
 * What are the problems with this approach ?!?
 * Well - "delete this" requires that the object in question was
 * allocated on the heap and not say, stack or data segment. In
 * that cause doing "delete this" may cause a segfault or maybe
 * even a silent bug which is worse at times.
 */

class A {
	private:
		int secret;
	public:
		A(void) {
			printf("This is the constructor\n");
		}
		~A(void) {
			printf("This is the destructor\n");
		}
		void doit(void) {
			// the next line must be the very LAST line
			// in this methods code (after any business
			// logic).
			delete this;
		}
};

// here is an object that causes infinite recursion in it's destructor...
class B {
	public:
		~B(void) {
			delete this;
		}
};

int main(int argc, char **argv, char **envp) {
	A* a=new A();
	a->doit();
	// this is a problem since the next allocation is on the stack and not
	// on the heap...
	//A aa;
	//aa.doit();
	// the next code will cause infinite recursion and error...
	//B* b=new B();
	//delete b;
	return EXIT_SUCCESS;
}
