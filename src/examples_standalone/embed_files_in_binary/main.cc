#include<stdio.h>

extern "C" {
	extern char _binary_data_txt_start;
	extern char _binary_data_txt_end;
}

int main(int argc,char** argv,char** envp) {
	char *p=&_binary_data_txt_start;
	while ( p != &_binary_data_txt_end ) putchar(*p++);
	return 0;
}
