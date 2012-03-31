#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is an example of the C trinary operator
 *
 * Mark Veltzer
 */

int main(int argc,char** argv, char** envp) {
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
	return EXIT_SUCCESS;
}
