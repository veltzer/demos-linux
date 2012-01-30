#include <stdio.h> // for printf(3)

int main() {
	int i=5;
	int& ri=i;
	printf("i is %d, ri is %d\n",i,ri);
	ri=6;
	printf("i is %d, ri is %d\n",i,ri);
	i=7;
	printf("i is %d, ri is %d\n",i,ri);
	return 0;
}
