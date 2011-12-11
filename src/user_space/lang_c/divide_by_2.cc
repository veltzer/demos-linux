#include <stdio.h> // for scanf(3)

int main() {
	int num;
	scanf("%d",&num);
	int i=0;
	while(num/2>0) {
		i++;
		num/=2;
	}
	printf("number of times divided is %d\n",i);
	return 0;
}
