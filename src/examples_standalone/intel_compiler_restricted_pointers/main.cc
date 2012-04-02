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

#include<stdio.h> // for vprintf(3)
#include<sys/time.h> // for gettimeofday(2)

#include<us_helper.h> // for micro_diff

/*
* Same example the example for gcc but now for the Intel compiler...
*/

#define myrestrict restrict
//#define myrestrict

void add_check_restrict(int* arr,int num, int* result) __attribute__ ((noinline));
void add_check_restrict(int* arr,int num, int* result) {
	if(result<arr || result>arr+num) {
		int temp=0;
		for(int i=0;i<num;i++) {
			temp+=arr[i];
		}
		*result=temp;
	} else {
		*result=0;
		for(int i=0;i<num;i++) {
			*result+=arr[i];
		}
	}
}

void add_no_restrict(int* arr,int num, int* result) __attribute__ ((noinline));
void add_no_restrict(int* arr,int num, int* result) {
	*result=0;
	for(int i=0;i<num;i++) {
		*result+=arr[i];
	}
}

void add_restrict(int* myrestrict arr,int num, int* myrestrict result) __attribute__ ((noinline));
void add_restrict(int* myrestrict arr,int num, int* myrestrict result) {
	*result=0;
	for(int i=0;i<num;i++) {
		*result+=arr[i];
	}
}

void add_temp(int* arr,int num, int* result) __attribute__ ((noinline));
void add_temp(int* arr,int num, int* result) {
	int temp=0;
	for(int i=0;i<num;i++) {
		temp+=arr[i];
	}
	*result=temp;
}

int main(int argc,char** argv,char** envp) {
	// prepare a large array
	const unsigned int array_size=1000000;
	int* arr=new int[array_size];
	int res;
	for(unsigned int i=0;i<array_size;i++) {
		arr[i]=random();
	}
	const unsigned int loop=1000;
	struct timeval t1, t2;

	printf("doing %d no restrict calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_no_restrict(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d restrict calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_restrict(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d temp calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_temp(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return(0);
}
