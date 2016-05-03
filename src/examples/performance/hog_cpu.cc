#include <stdio.h>

unsigned long long sum_it(unsigned long to) __attribute__((noinline));
unsigned long long sum_it(unsigned long to) {
	unsigned long long sum=0;
	for(unsigned long i=0;i<10000000;i++) {
		for(unsigned long j=0;j<10000000;j++) {
			sum+=j*j*j;
		}
	}
	return sum;
}

int main(int argc, char** argv, char** envp) {
	unsigned long long sum=0;
	for(unsigned long i=0;i<10000000;i++) {
		sum+=sum_it(10000000);
	}
	printf("sum is %llu\n", sum);
	return 0;
}
