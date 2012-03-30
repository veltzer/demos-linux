#include<stdio.h> // for fprintf(3), printf(3)
#include<math.h> // for sin(3)
#include<stdlib.h> // for atof(3)

/*
 * The function runs the sin function on data from the outside world because
 * otherwise the wiseass compiler would computer the sin function at compiler
 * time!
 */
int main(int argc, char **argv, char **envp) {
	if(argc!=2) {
		fprintf(stderr,"usage: main [x]\n");
		return -1;
	}
	double val=atof(argv[1]);
	printf("sin(%lf) is %lf\n",val,sin(val));
	return(0);
}
