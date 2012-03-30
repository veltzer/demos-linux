#include<stdio.h> // for printf(3)

/*
 * This example shows how to pass an array by reference in C
 *
 * 			Mark Veltzer
 */

void print_array(double* ar,int size) {
	int i;
	for(i=0;i<size;i++)
		printf("ar[%d]=%f\n",i,ar[i]);
}

void manipulate_array(double* ar,int size) {
	int i;
	for(i=0;i<size;i++) {
		// nicest
		//ar[i]=(double)i;
		// not nice but works
		//*(ar+i)=(double)i;
		*ar=(double)i;
		ar++;
	}
}

int main() {
	const int SIZE=10;
	double d[10]={0,0,0,0,0,0,0,0,0,0};
	print_array(d,SIZE);
	manipulate_array(d,SIZE);
	print_array(d,SIZE);
	return 0;
}
