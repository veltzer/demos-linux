/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>

/*
 * You can nm this file and see the weak symbol that is produced
 * by the compiler.
 */

template<class T> class A {
	public:
		int foo(T t);
};

template<class T> int A<T>::foo(T t) {
	int sum=0;
	for(unsigned int i=0;i<100;i++) {
		sum+=i*i;
	}
	return sum;
}

int main(int argc, char** argv, char** envp) {
	A<int> a;
	int sum=a.foo(0);
	return sum;
}
