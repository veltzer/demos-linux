#include<stdio.h> // for printf(3)
#include<stdlib.h> // for atoi(3)
#include<dlfcn.h> // for dlsym(3)

/*
 * This example shows how to override a symbol like atoi. We define our own atoi
 * but also call the original implementation.
 *
 * EXTRA_LIBS=-ldl
 *
 * 		Mark Veltzer
 */
int (*p_atoi)(const char* nptr);

void setup() __attribute__((constructor));
void setup() {
	p_atoi=(typeof(p_atoi))dlsym(RTLD_NEXT,"atoi");
}

int atoi(const char* nptr) {
	return p_atoi(nptr)/2;
}

int main(int argc,char** argv,char** envp) {
	printf("atoi of 45 is %d\n",atoi("45"));
	return EXIT_SUCCESS;
}
