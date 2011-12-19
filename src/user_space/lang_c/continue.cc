#include <stdio.h> // for printf(3)

/**
 * Compare the version that uses the 'continue' keyword with the version that
 * does not. Which one is shorter? more maintanable? easier to read? easier
 * to change? Sometimes continue is just the right answer. In kernel coding even
 * more so...
 *
 * 				Mark Veltzer
 */

int main() {	
	/* without continue... */
	printf("first round...\n");
	for(int i=0;i<100;i++) {
		if(i%5!=0) {
			if((i+1)%3==0) {
				printf("i is %d\n",i);
			}
		}
	}
	/* with continue... */
	printf("second round...\n");
	for(int i=0;i<100;i++) {
		if(i%5==0) continue; 
		if((i+1)%3!=0) continue;
		printf("i is %d\n",i);
	}
	return 0;
}
