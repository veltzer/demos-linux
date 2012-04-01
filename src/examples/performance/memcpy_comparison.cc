/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<stdio.h> // for printf(3)
#include<sys/time.h> // for gettimeofday(2)
#include<stdlib.h> // for malloc(3), rand(3), EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include<string.h> // for malloc(3)

#include<us_helper.h> // for micro_diff

/*
 * This example compares memcpy(3) to copy by loop...
 * As you can see memcpy is faster since it uses a specialized machine instruction to do the copying.
 * See the disassembly for more details.
 * If you uncomment the rand() code you will find that memcpy does not give you any slack: it does NOT
 * check the arguments passed to it for sanity. This is part of the philosophy of the GNU/Linux system.
 * The idea is that system programmers can take care of themselves and the APIs should be as fast
 * as possible to cater for good programmers.
 */
struct timeval t1, t2;
unsigned int loop;
unsigned int size;
char* buf1;
char* buf2;

void test_memcpy() {
	printf("doing %d memcpy\n",loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0;i<loop;i++) {
		memcpy(buf1,buf2,size);
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);
}

void test_imp1() {
	printf("doing %d copy int by int (implementation I)\n",loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i = 0;i < loop;i++) {
		for(unsigned int j=0;j<size/sizeof(int);j++) {
			((int*)buf1)[j]=((int*)buf2)[j];
		}
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);
}

void test_imp2() {
	printf("doing %d copy int by int (implementation II)\n",loop);
	CHECK_NOT_M1(gettimeofday(&t1, NULL));
	for(unsigned int i=0;i<loop;i++) {
		int* pbuf1=(int*)buf1;
		int* pbuf2=(int*)buf2;
		for(unsigned int j=0;j<size/sizeof(int);j++) {
			*pbuf1=*pbuf2;
			pbuf1++;
			pbuf2++;
		}
	}
	CHECK_NOT_M1(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);
}

int main(int argc,char** argv,char** envp) {
	if(argc!=3) {
		printf("usage: %s [loop] [size]\n",argv[0]);
		return EXIT_FAILURE;
	}

	loop=atoi(argv[1]);
	size=atoi(argv[2]);
	/*
	if(rand()<RAND_MAX) {
		buf1=NULL;
	}
	*/
	buf1=(char*)malloc(size);
	buf2=(char*)malloc(size);

	test_memcpy();
	test_imp1();
	test_imp2();
	test_memcpy();
	test_imp1();
	test_imp2();

	return EXIT_SUCCESS;
}
