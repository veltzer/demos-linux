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

#include<iostream> // for std::cout, std::endl
#include<typeinfo> // for typeid
#include<stdlib.h> // for EXIT_SUCCESS

// empty class - don't touch this.
class Empty {
};

class Person {
public:
	// ... Person members ...
	virtual ~Person() {}
};

class Employee : public Person {
	// ... Employee members ...
};

int main(int argc,char** argv,char** envp) {
	Person person;
	Employee employee;
	Person *ptr = &employee;
	// The string returned by typeid::name is implementation-defined
	std::cout << typeid(person).name() << std::endl; // Person (statically known at compile-time)
	std::cout << typeid(employee).name() << std::endl; // Employee (statically known at compile-time)
	std::cout << typeid(ptr).name() << std::endl; // Person * (statically known at compile-time)
	std::cout << typeid(*ptr).name() << std::endl; // Employee (looked up dynamically at run-time
	// because it is the dereference of a
	// pointer to a polymorphic class)
	std::cout << "sizeof(Person) is " << sizeof(Person) << std::endl;
	std::cout << "sizeof(Empty) is " << sizeof(Empty) << std::endl;
	return EXIT_SUCCESS;
}
