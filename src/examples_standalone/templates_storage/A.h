#include<stdio.h>

template<class T> class A {
public:
	T t;
	inline T setT(T it) {
		t = it;
		T sum = 0;
		for (int i = 0; i < it; i++) {
			sum += i;
		}
		return(sum);
	}
};
