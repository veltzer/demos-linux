#include <stdio.h> // for printf(3)
#include <ctype.h> // for isgraph(3)

/*
 *      This is an example to show how best to print characters on the screen...
 *
 *              Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	for (int c = 0; c < 256; c++) {
		if (isgraph(c)) {
			printf("asc %d is %c\n", c, c);
		} else {
			printf("asc %d is non graphic\n", c);
		}
	}
	return(0);
}
