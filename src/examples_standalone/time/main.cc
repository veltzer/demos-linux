#include"time.hh"
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	Time t;
	t.setTime(14,56,32);
	t.printStd();
	t.printMilitary();
	for(int i=0;i<1000000;i++)
		t.Tick();
	t.printStd();
	t.printMilitary();
	return EXIT_SUCCESS;
}
