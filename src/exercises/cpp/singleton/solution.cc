#include "singleton.hh"


Singleton* Singleton::instance=nullptr;

Singleton::Singleton(double ia, double ib):a(ia), b(ib) {}

Singleton* Singleton::get_instance() {
	if(instance==nullptr) {
		instance=new Singleton(4,5);
	}
	return instance;
}

int main() {
}
