#include <stdio.h> // for scanf(3), putchar(3), perror(3)
#include <unistd.h> // for sleep(3)

int main() {
	int num;
	int ret=scanf("%d",&num);
	if(ret!=1) {
		perror("scanf");
		return -1;
	}
	while(1) {
		int i;
		int u;
		for(i=0,u=num-1;i<num;i++,u--) {
			int j;
			for(j=0;j<u;j++) {
				putchar(' ');
			}
			for(j=0;j<i+1;j++) {
				putchar('*');
				putchar(' ');
			}
			for(j=0;j<u;j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			usleep(100000);
		}
		for(i=num-1,u=0;i>=0;i--,u++) {
			int j;
			for(j=0;j<u;j++) {
				putchar(' ');
			}
			for(j=0;j<i+1;j++) {
				putchar('*');
				putchar(' ');
			}
			for(j=0;j<u;j++) {
				putchar(' ');
			}
			putchar('\r');
			fflush(stdout);
			usleep(100000);
		}
	}
	return 0;
}
