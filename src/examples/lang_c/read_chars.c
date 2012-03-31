#include<stdio.h> // for printf(3), getchar(3)
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	char c=getchar();
	while(c!=EOF) {
		printf("got a single character whose code is %d, and value is %c\n",c,c);
		c=getchar();
	}
	printf("Got EOF, you probably hit CTRL+D\n");
	return EXIT_SUCCESS;
}
