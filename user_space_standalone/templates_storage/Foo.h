#ifndef __Foo_h
#define __Foo_h

template<class T> class Foo {
protected:
	T t;
public:
	void setT(T it) {
		t = it;
	}
};

#endif // __Foo_h
