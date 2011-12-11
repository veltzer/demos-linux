#include <stdio.h> // for printf(3)
#include <string.h> // for memcpy(3)

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
}
void Employee::setSalary(int newsalary) {
	salary=newsalary;
}

///////////////////////////////
// This is in the using part //
///////////////////////////////
int main() {
	Employee e;
	int* p=(int*)&e;
	*p=1200;
	e.printSalary();
	return 0;
}
