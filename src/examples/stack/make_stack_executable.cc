#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <unistd.h> // for pause(2)
#include <sys/mman.h> // for mmap(2)

int main(int argc, char** argv, char** envp) {
	/*
	void* p=mmap(NULL, 4096, PROT_EXEC | PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(p==MAP_FAILED) {
		perror("this is the error");
		fprintf(stderr, "mmap failed");
		return EXIT_FAILURE;
	}
	*/
	const unsigned int num_pages=35;
	//void* p=get_stack_address();
	//unsigned int stack_size=140*4096;
	void* o=&argc;
	printf("o is at %p\n",o);
	unsigned int p_int=(unsigned int)&argc & (unsigned int)~4095;
	p_int+=4*4096;
	p_int-=num_pages*4096;
	void* p=(void*)p_int;
	printf("stack is at %p\n",p);
	int ret=mprotect(p, num_pages*4096, PROT_READ | PROT_WRITE | PROT_EXEC);
	if(ret==-1) {
		perror("this is the error");
		return EXIT_FAILURE;
	}
	while(true) {
		pause();
		printf("hey, I got a signal!\n");
	}
	return EXIT_SUCCESS;
}
