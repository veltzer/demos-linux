#include <stdio.h> // for printf(3)

/*
 * This example shows how to step out of the array bounds, in this case
 * in order to step over some other variable. Note that if you do not
 * make the other number (in this case 'num') materialize by taking a reference
 * to it then it will not materialize and a bug will not be seen (although a bug
 * will occur!).
 *
 * 			Mark Veltzer
 */

int main() {
	int a[10];
	int num=1995;
	printf("%p %p\n",a,&num);
	for(int i=0;i<10;i++) {
		a[i]=i;
	}
	for(int i=0;i<10;i++) {
		printf("a[%d]=%d\n",i,a[i]);
	}
	printf("num is %d\n",num);
	a[10]=2011;
	for(int i=0;i<10;i++) {
		printf("a[%d]=%d\n",i,a[i]);
	}
	printf("num is %d\n",num);
	return 0;
}
