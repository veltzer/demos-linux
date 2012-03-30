#include<stdio.h> // for printf(3)

int main() {
	int c=1;
	int d=c==1 ? 4 : 5;
	printf("d is %d\n",d);
	/*
	if(c==1) {
		d=4;
	} else {
		d=5;
	}
	*/
	return 0;
}
