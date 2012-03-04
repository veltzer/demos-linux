#include <iostream> // for std::cout, std::endl
#include <typeinfo> // for typeid

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
}
