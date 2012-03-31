#include<stdio.h> // for scanf(3), perror(3)

int main(int argc, char **argv, char **envp) {
	int num;
	int ret=scanf("%d",&num);
	if(ret!=1) {
		perror("scanf");
		return -1;
	}
	int i=0;
	while(num/2>0) {
		i++;
		num/=2;
	}
	printf("number of times divided is %d\n",i);
	return 0;
}
