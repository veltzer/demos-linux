#include <stdio.h> // for printf(3), getchar(3)

int main() {
	char c=getchar();
	while(c!=EOF) {
		printf("got a single character whose code is %d, and value is %c\n",c,c);
		c=getchar();
	}
	printf("Got EOF, you probably hit CTRL+D\n");
	return 0;
}
