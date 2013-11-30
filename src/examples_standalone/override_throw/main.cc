#include <iostream>	// for std::exception, std::cout
#include <stdlib.h>	// for EXIT_SUCCESS

class MyException : public std::exception {
};

int main() {
	try {
		throw MyException();
	}catch(MyException e) {
		std::cout << "in the catch" << std::endl;
	}
	return EXIT_SUCCESS;
}
