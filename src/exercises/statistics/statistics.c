#include <stdio.h>	// for printf(3), scanf(3), fflush(3)
#include <stdlib.h>	// for EXIT_SUCCESS, malloc(3), free(3)
#include <us_helper.h>	// for CHECK_INT()

float get_average(float* p, int size) {
	int i;
	float sum=0;
	for(i=0; i<size; i++) {
		sum+=p[i];
		/* sum+=*(p+i); */
	}
	return sum/size;
}

int main(int argc, char** argv, char** envp) {
	int size;
	float* p;
	int i;

	printf("How many elements do you have? ");
	// there is not need to flush since scanf will flush
	// as well
	/* fflush(stdout); */
	CHECK_INT(scanf("%d",&size),1);
	p=(float*)malloc(sizeof(float)*size);
	for(i=0; i<size; i++) {
		printf("please give me the %d'th element: ", i);
		CHECK_INT(scanf("%f",&p[i]),1);
		/* scanf("%f",p+i); */
	}
	printf("the average is %f\n",get_average(p, size));
	free(p);
	return EXIT_SUCCESS;
}
