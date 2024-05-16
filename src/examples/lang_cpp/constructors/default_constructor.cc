/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2024 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <iostream>	// for std::cout, std::endl
#include <future>	// for std::future

/*
 * This example shows that you get a free constructor for any class you write
 * which initializes it's data to 0.
 * (under certain conditions).
 */

class Foo {
	private:
		int n=0;
		void* p=NULL;
	public:
		void print() {
			std::cout << "n is " << n << std::endl;
			std::cout << "p is " << std::hex << std::showbase << p << std::endl;
		}
};

int main() {
	Foo f;
	f.print();
	return EXIT_SUCCESS;
}
