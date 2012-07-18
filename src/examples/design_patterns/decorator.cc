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
#include<iostream> // for std::cout, std::endl

/*
* This example shows the decorator pattern implemented in C++.
*/

class Base {
	public:
		virtual void act(void) {
			std::cout << "in Base" << std::endl;
		}
};
class Derived1:public Base {
	private:
		Base& base;
	public:
		Derived1(Base& ibase):base(ibase) {
		}
		virtual void act(void) {
			base.act();
			std::cout << "in Derived1" << std::endl;
		}
};
class Derived2:public Base {
	private:
		Base& base;
	public:
		Derived2(Base& ibase):base(ibase) {
		}
		virtual void act(void) {
			base.act();
			std::cout << "in Derived2" << std::endl;
		}
};

int main(int argc,char** argv,char** envp) {
	Base base;
	Derived1 derived1(base);
	Derived2 derived2(derived1);
	derived2.act();
	return EXIT_SUCCESS;
}
