#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {
	const int bufsize=1000;
	int buf[bufsize];
	for(int i=0;i<bufsize;i++) {
		buf[i]=i;
	}
	const int very_long=1000000000;
	double sum=0;
	for(int i=0;i<very_long;i++) {
		if(rand()%2==0) {
			sum+=buf[i%bufsize];
		} else {
			sum-=buf[i%bufsize];
		}
	}
	printf("sum is %lf\n", sum);
	return 0;
}

