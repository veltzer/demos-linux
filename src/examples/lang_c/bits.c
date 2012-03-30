#include<stdio.h>
#include<string.h>

/*
 * This is to check some assumptions on bit operations
 *
 *		Mark Veltzer
 */

#define COMB(a, b) (a | b)
const unsigned int cone = 0x0000f000;
const unsigned int ctwo = 0x0000f001;

#define one 0x00010000
#define two 0x706

int main(int argc, char **argv, char **envp) {
	printf("COMB(0x800,%d) is %d\n", cone, COMB(0x800, cone));
	printf("COMB(0x801,%d) is %d\n", cone, COMB(0x801, cone));
	printf("COMB(%d,%d) is %d\n", one, two, COMB(one, two));
	return(0);
}
