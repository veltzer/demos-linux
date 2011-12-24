#include<stdio.h>

int main(int argc,char** argv,char** envp) {
	int counter=0;
	while(1) {
		printf("hi, counter is %d\n",counter);
		sleep(1);
		counter++;
	}
	return 0;
}
