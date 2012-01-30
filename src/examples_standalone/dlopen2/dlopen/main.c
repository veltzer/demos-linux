#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
	void *h = dlopen("libadd.so", RTLD_NOW);
	void *sym = dlsym(h, "func");

	if (!sym) {
		fprintf(stderr, "func is not found\n");
		exit(1);
	}
	int (*f)(int, int);
	f = ((int(*) (int, int))sym);
	int result = f(2, 2);
	printf("2+2 is %d\n", result);
	dlclose(h);
	return(0);
}
