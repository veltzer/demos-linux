#include <stdio.h> // for printf(3)

/**
 * Compare the version that uses the 'break' keyword with the version that
 * does not. Which one is shorter? more maintanable? easier to read? easier
 * to change? Sometimes break is just the right answer.
 *
 * 				Mark Veltzer
 */

int main() {
	/* without break */
	int early_break=0;
	int i;
	for(i=0;i<100 && !early_break;i++) {
		if(i==5)
			early_break=1;
		else
			printf("i is %d\n",i);
	}
	/* with break */
	for(i=0;i<100;i++) {
		if(i==5) break;
		printf("i is %d\n",i);
	}
	return 0;
}
