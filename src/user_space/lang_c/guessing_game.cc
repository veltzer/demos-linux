#include <stdio.h> // for printf(3), scanf(3)
#include <stdlib.h> // for rand(3)

int main() {
	int rand_number;
	int cont=1;
	while(cont) {
		rand_number=rand()%100;
		//printf("rand_number is %d\n",rand_number);
		int guess=-1;
		while(guess!=rand_number) {
			printf("what is your guess ? ");
			scanf("%d",&guess);
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
		scanf("%d",&cont);
	}
	return 0;
}
