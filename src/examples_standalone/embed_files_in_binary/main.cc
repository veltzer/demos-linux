#include<stdio.h> // for putchar(3)
#include<stdlib.h> // for EXIT_SUCCESS

extern "C" {
	extern char _binary_data_txt_start;
	extern char _binary_data_txt_end;
}

int main(int argc,char** argv,char** envp) {
	char *p=&_binary_data_txt_start;
	while(p!=&_binary_data_txt_end)
		putchar(*p++);
	return EXIT_SUCCESS;
}
