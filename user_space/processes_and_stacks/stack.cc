#include<stdio.h>
#include<string.h>

void func(void) {
	const unsigned int m=10;
	const unsigned int size=m*1024*1024;
	char data[size];
	memset(data,0,size);
}

int main(int argc,char** argv,char** envp) {
	func();
	return 0;
}
