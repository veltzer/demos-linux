#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS, rand(3)

int main(int argc,char** argv,char** envp) {
	int a;
	int i;
	printf("the address of a is %p\n",&a);
	printf("the address of i is %p\n",&i);
	int* pi=&a+1;
	printf("pi is %p\n",pi);
	/*
	if(rand()<RAND_MAX) {
		pi+=1;
		printf("pi is %p\n",pi);
	}
	*/
	for(i=0;i<100;i++) {
		//printf("i is %d\n",i);
		//printf("*pi is %d\n",*pi);
		if(i==10) {
			*pi=700;
		}
		//__sync_synchronize();
		//asm volatile("":::"memory");
		asm volatile("":"=g" (i)::);
	}
	printf("after the loop\n");
	printf("i is %d\n",i);
	printf("*pi is %d\n",*pi);
	return EXIT_SUCCESS;
}
