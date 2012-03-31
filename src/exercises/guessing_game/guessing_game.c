#include<stdio.h> // for printf(3), scanf(3), perror(3)
#include<stdlib.h> // for rand(3), EXIT_SUCCESS
#include<errno.h> // for errno

int main(int argc,char** argv,char** envp) {
	int rand_number;
	int cont=1;
	while(cont) {
		rand_number=rand()%100;
		//printf("rand_number is %d\n",rand_number);
		int guess=-1;
		while(guess!=rand_number) {
			printf("what is your guess ? ");
			int ret=scanf("%d",&guess);
			if(ret!=1) {
				perror("scanf");
				return -1;
			}
			if(guess<rand_number) {
				printf("guess higher...\n");
			}
			if(guess>rand_number) {
				printf("guess lower...\n");
			}
			if(guess==rand_number) {
				printf("RIGHT!!!\n");
			}
		}
		printf("want to play more ? ");
		int ret=scanf("%d",&cont);
		if(ret!=1) {
			int err=errno;
			perror("scanf");
			return err;
		}
	}
	return EXIT_SUCCESS;
}
