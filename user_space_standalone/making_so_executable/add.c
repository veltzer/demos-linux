#include<stdio.h> // for printf(3)
#include<unistd.h> // for write(2)

int func(int a, int b) {
	return(a + b);
}


/*
void do_init(void) __attribute__((constructor));
void do_init(void) {
	printf("Hello\n");
}
*/

int my_entry(int argc,char** argv,char** envp) {
	/*
	_exit(1);
	int ret=write(1,"hello\n",6);
	if(ret!=6) {
		_exit(1);
	}
	//printf("hello from the lib...");
	*/
	return 0;
}
