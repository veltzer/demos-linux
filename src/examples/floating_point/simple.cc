#include<stdio.h> // for printf(3), snprintf(3)
#include<stdlib.h> // for atof(3)

// This example is very simple. It shows how a specific
// integer value cannot be represented as float.
//
// Why was the specific value chosen (2^24+1)?
// Because a floating point number on a 32 bit system is divided
// into 1+7+24=32 bits:
// 	1 bit for the sign
// 	7 bits for the exponent
// 	24 bits for the number
// In regular number range (whole numbers) of small numbers only
// the sign and number are used. So this means that the floating
// point representation should start to mess up at the 2^24 point
// (and -2^24).
//
// This test works on an intel 32 bit system. On other systems
// your results may be different as the definition of int and float
// may change or the math hardware may work slightly differently.
//
// 	Mark Veltzer

int main(int argc,char** argv,char** envp) {
	// example with built in compiler conversions
	int i=16777217; // 2^24+1
	// the volatile is neccessary so the compiler will not eliminate the entire code!
	volatile float f=i;
	int back_i=f;
	if(back_i==i) {
		printf("they are the same...\n");
	} else {
		printf("they are different!\n");
		printf("i is %d\n",i);
		printf("f is %f\n",f);
		printf("back_i is %d\n",back_i);
	}
	// example with atof
	int i2=16777217;
	char str[256];
	snprintf(str,256,"%d",i2);
	float f2=atof(str);
	int back_i2=f2;
	if(back_i2==i2) {
		printf("they are the same...\n");
	} else {
		printf("they are different!\n");
		printf("i2 is %d\n",i2);
		printf("f2 is %f\n",f2);
		printf("back_i2 is %d\n",back_i2);
	}
	return EXIT_SUCCESS;
}
