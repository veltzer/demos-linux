#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv, char **envp) {
	void *h = dlopen("libadd.so", RTLD_NOW);

	if (h == NULL) {
		perror("problem with dlopen");
		exit(errno);
	}
	void *sym = dlsym(h, "func");
	if (sym == NULL) {
		perror("problem with dlsym");
		exit(errno);
	}
	int (*f)(int, int);
	f = ((int(*) (int, int))sym);
	int result = f(2, 2);
	printf("2+2 is %d\n", result);
	int rc = dlclose(h);
	if (rc) {
		perror("problem with dlclose");
		exit(errno);
	}
	return(0);
}
