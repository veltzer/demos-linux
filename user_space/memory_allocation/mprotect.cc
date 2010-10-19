#include<stdio.h> // for printf(3), perror(3)
#include<stdlib.h> // for malloc(3), free(3), exit(3)
#include<sys/mman.h> // for mprotect(2)
#include<signal.h> // for sigaction(2)
#include<malloc.h> // for memalign(3)
#include<errno.h> // for errno
#include<unistd.h> // for getpagesize(2)

#define HANDLE_SEGV

#ifdef HANDLE_SEGV
static void handler(int sig, siginfo_t *si, void *unused) {
	printf("Got SIGSEGV at address: 0x%lx\n",(long) si->si_addr);
	exit(EXIT_FAILURE);
}
#endif // HANDLE_SEGV

static void register_handler() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		perror("sigaction");
		exit(errno);
	}
}

const void* mymalloc(unsigned int size,int protect) {
	if(protect) {
		void* buf=(void*)memalign(getpagesize(),size);
		if(buf==NULL) {
			perror("memalign");
			exit(errno);
		}
		int val=mprotect(buf,size,PROT_NONE);
		if(val==-1) {
			perror("error in mprotect");
			exit(1);
		}
		return buf;
		/*
 		// does not work for some reason
		int ps=getpagesize();
		void* ret=malloc(ps+size);
		int iret=(int)ret;
		void* newaddress=(void*)((iret+ps-1) & ~(ps-1));
		size_t sz=size;
		int val=mprotect(newaddress,sz,PROT_NONE);
		if(!val) {
			perror("error in mprotect");
			exit(1);
		}
		return newaddress;
		*/
	} else {
		return malloc(size);
	}
}

void bug_doing_write(char* buf) {
	int i;
	for(i=0;i<10;i++) {
		buf[i]=0;
	}
}

void bug_doing_read(char* buf) {
	int i;
	for(i=0;i<10;i++) {
		char t __attribute__((unused))=buf[i];
	}
}

int main(int argc,char** argv,char** envp) {
#ifdef HANDLE_SEGV
	register_handler();
#endif // HANDLE_SEGV

	char* buffer=(char*)mymalloc(10,1);

	bug_doing_write(buffer);
	bug_doing_read(buffer);

	free(buffer);
	return(0);
}
