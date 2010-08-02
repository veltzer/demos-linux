#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main(int argc, char **argv, char **envp) {
	if (argc != 2) {
		fprintf(stderr, "%s: usage %s arg\n", argv[0], argv[0]);
		exit(-1);
	}
	const double arg = atof(argv[1]);
	const double result = myfunc(arg);
	printf("result is %lf\n", result);
	return(0);
}
