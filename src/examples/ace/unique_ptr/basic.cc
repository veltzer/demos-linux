/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2025 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <iostream>	// for cout, endl
#include <cstdlib>	// for EXIT_SUCCESS
#include <memory>

using namespace std;

/*
 * This example demonstrates the use of auto_ptr which takes
 * care of doing the "delete" when the auto_ptr object is destroyed
 * (goes out of scope)
 *
 * EXTRA_COMPILE_CMD=pkg-config --cflags ACE
 * EXTRA_LINK_CMD=pkg-config --libs ACE
 */

class A{
private:
	const char *p;

public:
	A(const char *ip) : p(ip) {
		cout << "in constructor of " << p << endl;
	}
	~A() {
		cout << "in destructor of " << p << endl;
	}
};

int main() {
	unique_ptr<A> a(new A("a"));
	new A("b");
	return EXIT_SUCCESS;
}
