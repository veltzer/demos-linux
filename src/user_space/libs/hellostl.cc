#include <vector>
#include <iostream>

/*
 *      This program demonstrates the use of STL (Standard Template Library).
 *
 *	Mark Veltzer
 *
 */
int main(void) {
	std::vector<int> v(3);

	v[0] = 5;
	v[1] = 8;
	v[2] = 9;
	std::cout << v[0] << "-" << v[1] << "-" << v[2] << "\n";
	return(0);
}


// we need this because STL assumes we are instantiating all templates...:)
// int larger programs you may need a whole file of these...
//template int* fill_n<int *, unsigned int, int>(int *, unsigned int, int const &);
template int *std::fill_n<int *, unsigned, int>(int *, unsigned, int const&);
