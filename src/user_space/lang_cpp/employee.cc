#include <stdio.h> // for printf(3)
#include <string.h> // for memcpy(3)
#include <iostream>

////////////////////////////
// This is in the HH file //
////////////////////////////

class Employee {
	private:
		int salary;
	public:
		void printSalary();
		void setSalary(int newsalary);
};

////////////////////////////
// This is in the CC file //
////////////////////////////
void Employee::printSalary() {
	printf("my salary is %d\n",salary);
	std::cout << "Hello";
}
void Employee::setSalary(int newsalary) {
	salary=newsalary;
}

///////////////////////////////
// This is in the using part //
///////////////////////////////
int main() {
	Employee e;
	e.setSalary(7);
	e.printSalary();
	return 0;
}
