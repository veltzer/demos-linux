#include<stdio.h> // for perror(3)
#include<stdlib.h> // for exit(3), posix_memalign(3)
#include<unistd.h> // for getpagesize(2)
#include<sys/mman.h> // for mprotect(2)

// this code simulates a memory corruption
void bad_code(void* precious_data) {
	char* p=(char*)precious_data;
	p[0]=5;
}

void protect_me(void* precious_data,size_t len) {
	if(mprotect(precious_data,len,PROT_READ)==-1) {
		perror("mprotect");
		exit(1);
	}
}

int main(int argc,char** argv,char** envp) {
	void* precious_data;
	const unsigned int size=8192;
	if(posix_memalign(&precious_data,getpagesize(),size)==-1) {
		perror("posix_memalign");
		exit(1);
	}
	protect_me(precious_data,size);
	bad_code(precious_data);
	return EXIT_SUCCESS;
}
