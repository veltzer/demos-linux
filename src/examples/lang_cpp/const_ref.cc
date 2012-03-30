#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	//const int i=5;
	//const int* pi=&i;
	//const int& ri=i;
	// next lines produce compilation errors...
	// i=8;
	//*pi=7;
	//ri=8;
	return EXIT_SUCCESS;
}
