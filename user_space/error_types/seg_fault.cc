#include <stdio.h> // for fprintf(3)

void func() {
	int i;
	const char* y __attribute__((unused))="123123dsfshfdkj$%^$%&^^%&@";
	for(i=0;i<100;i++) {
		fprintf(stderr,"i is %d\n",i);
	}
}

int main(int argc,char** argv,char** envp) {
	char* f=(char*)func;
	f++;
	f++;
	void (*g)()=(void (*)())f;
	g();
	return(0);
}
