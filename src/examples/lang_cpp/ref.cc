#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

int main() {
	int i=5;
	int& ri=i;
	printf("i is %d, ri is %d\n",i,ri);
	ri=6;
	printf("i is %d, ri is %d\n",i,ri);
	i=7;
	printf("i is %d, ri is %d\n",i,ri);
	return EXIT_SUCCESS;
}
