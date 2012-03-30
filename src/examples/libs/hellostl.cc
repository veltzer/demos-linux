#include <vector> // for std::vector
#include <iostream> // for std::cout, std::endl
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This program demonstrates the use of STL (Standard Template Library).
 *
 *	Mark Veltzer
 *
 */
int main(void) {
	std::vector<int> v(3);

	v[0] = 5;
	v[1] = 8;
	v[2] = 9;
	std::cout << v[0] << "-" << v[1] << "-" << v[2] << std::endl;
	return EXIT_SUCCESS;
}
