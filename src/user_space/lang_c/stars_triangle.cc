#include <stdio.h> // for scanf(3), putchar(3)
#include <unistd.h> // for sleep(3)

int main() {
	int num;
	scanf("%d",&num);
	int u;
	while(1) {
		for(int i=0,u=num-1;i<num;i++,u--) {
			for(int j=0;j<u;j++) {
				putchar(' ');
			}
			for(int j=0;j<i+1;j++) {
				putchar('*');
				putchar(' ');
			}
			for(int j=0;j<u;j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			usleep(100000);
		}
		for(int i=num-1,u=0;i>=0;i--,u++) {
			for(int j=0;j<u;j++) {
				putchar(' ');
			}
			for(int j=0;j<i+1;j++) {
				putchar('*');
				putchar(' ');
			}
			for(int j=0;j<u;j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			usleep(100000);
		}
	}
	return 0;
}
