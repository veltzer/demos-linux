#include<stdio.h> // for printf(3), scanf(3), perror(3)
#include<stdlib.h> // for exit(3)

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

long int get_number() {
	long int ret;
	int err_code=scanf("%ld",&ret);
	if(err_code!=1) {
		perror("scanf");
		exit(-1);
	}
	return ret;
}

int main() {
	printf("enter the first number: ");
	long int num1=get_number();
	printf("enter the second number: ");
	long int num2=get_number();
	long int mul=num1*num2;
	printf("num1 is %ld, %d\n",num1,digit_num(num1));
	printf("num2 is %ld, %d\n",num2,digit_num(num2));
	printf("mul is %ld, %d\n",mul,digit_num(mul));
	return 0;
}
