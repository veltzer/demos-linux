#include<iostream> // for std::cout, std::endl
#include<stdlib.h> // for EXIT_SUCCESS

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
	std::cout << "Hello" << salary << std::endl;
}
void Employee::setSalary(int newsalary) {
	salary=newsalary;
}

///////////////////////////////
// This is in the using part //
///////////////////////////////
int main(int argc,char** argv,char** envp) {
	Employee e;
	e.setSalary(7);
	e.printSalary();
	return EXIT_SUCCESS;
}
