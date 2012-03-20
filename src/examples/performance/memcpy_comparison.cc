#include <stdio.h> // for printf(3)
#include <sys/time.h> // for gettimeofday(2)
#include <stdlib.h> // for malloc(3)
#include <string.h> // for malloc(3)

#include "us_helper.hh" // for micro_diff

/*
 * This example compares memcpy(3) to copy by loop...
 * TODO:
 *
 * 		Mark Veltzer
 * EXTRA_LIBS=
 */

int main(int argc, char **argv, char** envp) {
	struct timeval t1, t2;
	const unsigned int loop=10000000;
	const unsigned int size=200;
	char* buf1=(char*)malloc(size);
	char* buf2=(char*)malloc(size);
	/*
	if(rand()<RAND_MAX) {
		buf1=NULL;
	}
	*/

	printf("doing %d memcpy\n",loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for (unsigned int i = 0;i < loop;i++) {
		memcpy(buf1,buf2,size);
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d copy int by int\n",loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for (unsigned int i = 0;i < loop;i++) {
		int* pbuf1=(int*)buf1;
		int* pbuf2=(int*)buf2;
		for(unsigned int j=0;j<size/sizeof(int);j++) {
			*pbuf1=*pbuf2;
			pbuf1++;
			pbuf2++;
			//((int*)buf1)[j]=((int*)buf2)[j];
		}
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	return 0;
}
