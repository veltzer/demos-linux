#include"time.hh"

int main() {
	Time t;
	t.setTime(14,56,32);
	t.printStd();
	t.printMilitary();
	for(int i=0;i<1000000;i++)
		t.Tick();
	t.printStd();
	t.printMilitary();
	return 0;
}
