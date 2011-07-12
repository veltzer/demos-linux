#include<stdio.h> // for printf(3), snprintf(3)
#include<stdlib.h> // for atof(3), atoi(3)
#include<values.h> // for MAXINT, MININT, MAXFLOAT, MINFLOAT

#include<Stat.h> // our own statistics gathering object

// this example explores the representational issues of the floating point
// system in the mathematical co-processor.
//
// On a 32 bit system try the number 16777216 as the limit of presentation of
// floating point...
// Up to this number the floating point system representat integers precisely
// using the mantisa. After this number it goes into using the exponent and therefor
// becomes un-exact.
//
// Conclusions:
// atof behaves differently from float=int.
//
// 	Mark Veltzer

#define DO_FLOAT
//#define DO_DOUBLE
//#define DO_ATOF

int main(int argc,char** argv,char** envp) {
	// first lets print the limits
	printf("MAXINT is %d\n",MAXINT);
	printf("MININT is %d\n",MININT);
	printf("MAXFLOAT is %f\n",MAXFLOAT);
	printf("MINFLOAT is %f\n",MINFLOAT);
	printf("MAXDOUBLE is %lf\n",MAXDOUBLE);
	printf("MINDOUBLE is %lf\n",MINDOUBLE);
	unsigned int num_bins=100;
	unsigned long range=(unsigned long)MAXINT-(unsigned long)MININT;
	double dbin_size=(double)range/(double)num_bins;
	unsigned long bin_size=dbin_size;
	printf("bin_size is %lu\n",bin_size);
	unsigned int bin_counter=0;
	// collectors - one for float and one for double
#ifdef DO_FLOAT
	Stat s_f(num_bins,dbin_size,0);
#endif // DO_FLOAT
#ifdef DO_DOUBLE
	Stat s_d(num_bins,dbin_size,0);
#endif // DO_DOUBLE
#ifdef DO_ATOF
	Stat s_atof(num_bins,dbin_size,0);
#endif // DO_ATOF
	for(int i=MININT;i<MAXINT;i++) {
		#ifdef DO_FLOAT
		// direct conversion of int to float
		// the 'volatile' is necessary in order that the compiler
		// will not optimize the next two assignments and will do away
		// with the intermediate float...
		volatile float f=i;
		int back_i_f=f;
		if(i!=back_i_f) {
			s_f.accept(i);
		}
		#endif // DO_FLOAT
		#ifdef DO_DOUBLE
		// same thing for double although double covers the range of int
		// pretty well...
		volatile double d=i;
		int back_i_d=d;
		if(i!=back_i_d) {
			s_d.accept(i);
		}
		#endif // DO_DOUBLE
		#ifdef DO_ATOF
		// now print i into a string buffer, atof it back to float and convert
		char string_of_i[128];
		snprintf(string_of_i,128,"%d",i);
		float f2=atof(string_of_i);
		int back_i_atof=f2;
		if(i!=back_i_atof) {
			s_atof.accept(i);
		}
		#endif // DO_ATOF
		// progress report...
		bin_counter++;
		if(bin_counter==bin_size) {
			printf("new bin %d\n",i);
			// reset counters
			bin_counter=0;
		}
	}
	#ifdef DO_FLOAT
	s_f.print();
	#endif // DO_FLOAT
	#ifdef DO_DOUBLE
	s_d.print();
	#endif // DO_DOUBLE
	#ifdef DO_ATOF
	s_atof.print();
	#endif // DO_ATOF
	return 0;
}
