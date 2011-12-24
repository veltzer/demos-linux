#include <stdio.h> // for printf(3)
#include <stdlib.h> // for atoi(3)
#include <sys/types.h>
#include <unistd.h>
#include "add.h"

int main(int argc,char** argv,char** envp) {
	int a,b,i;
	if(argc>2) {
		a=atoi(argv[1]);
		b=atoi(argv[2]);
	} else {
		a=getpid();
		b=getppid();
	}
	for(i=0;i<a;i++) {
		if(i%555==0) {
			*(char*)0=0;
		}
	}
	int sum=add(a,b);
	printf("did you know that %d+%d=%d ?\n",a,b,sum);
	return 0;
}
