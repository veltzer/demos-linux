#include <stdio.h> // for printf(3)

int main() {
	const int i=5;
	const int* pi=&i;
	const int& ri=i;
	*pi=7;
	ri=8;
	return 0;
}
