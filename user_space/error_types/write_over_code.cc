#include <stdio.h>

void my_func() {
	int x=7;
	printf("x is %d\n",x);
}

int main(int argc,char** argv,char** envp) {
	int* t=(int*)&my_func;
	*t=8;
	return(0);
}
