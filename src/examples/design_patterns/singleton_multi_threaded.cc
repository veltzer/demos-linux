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
#include <iostream>
#include <mutex>

class Singleton {
public:
	static Singleton* getInstance() {
		if (instance == nullptr) {
			std::lock_guard<std::mutex> lock(mutex);
			if (instance == nullptr) {
				instance = new Singleton();
			}
		}
		return instance;
	}

	void someOperation() {
		std::cout << "Performing some operation in the Singleton." << std::endl;
	}

private:
	Singleton() {
		std::cout << "Singleton constructor called." << std::endl;
	}
	~Singleton() {
		std::cout << "Singleton destructor called." << std::endl;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static Singleton* instance;
	static std::mutex mutex;
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mutex;

int main() {
	Singleton* instance1 = Singleton::getInstance();
	instance1->someOperation();

	Singleton* instance2 = Singleton::getInstance();
	instance2->someOperation();

	return EXIT_SUCCESS;
}
