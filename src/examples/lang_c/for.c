#include<stdio.h> // for printf(3)

int main() {
	int j=0;
	while(j<100) {
		printf("j is %d\n",j);
		j++;
	}
	int i;
	for(i=0;i<100;i++) {
		printf("i is %d\n",i);
	}
	printf("i is %d\n",i);
	return 0;
}
