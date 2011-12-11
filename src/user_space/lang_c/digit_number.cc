#include <stdio.h> // for printf(3), scanf(3)

int digit_num(long int l) {
	int digits;
	if(l<0) {
		digits=1;
		l=-l;
	} else {
		digits=0;
	}
	while(l>0) {
		l/=10;
		digits++;
	}
	return digits;
}

int main() {
	long int num1,num2;
	printf("enter the first number: ");
	scanf("%ld",&num1);
	printf("enter the second number: ");
	scanf("%ld",&num2);
	long int mul=num1*num2;
	printf("num1 is %ld, %d\n",num1,digit_num(num1));
	printf("num2 is %ld, %d\n",num2,digit_num(num2));
	printf("mul is %ld, %d\n",mul,digit_num(mul));
	return 0;
}
