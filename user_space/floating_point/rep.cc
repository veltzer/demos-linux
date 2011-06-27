#include<stdio.h> // for printf(3), snprintf(3)
#include<stdlib.h> // for atof(3), atoi(3)

// this example explores the representational issues of the floating point
// system in the mathematical co-processor.
//
// On a 32 bit system try the number 16777216 as the limit of presentation of
// floating point...
// Up to this number the floating point system representat integers precisely
// using the mantisa. After this number it goes into using the exponent and therefor
// becomes un-exact.
//
// 				Mark Veltzer

int main(int argc,char** argv,char** envp) {
	float f=atof(argv[1]);
	//int precision=atoi(argv[2]);
	//const int fmtmax=128;
	//char fmt[fmtmax];
	//snprintf(fmt,fmtmax,"%.%dlf\n",precision);
	//printf((const char*)fmt,f);
	printf("%.70f\n",f);
	return 0;
}
