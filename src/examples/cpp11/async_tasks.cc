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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for stringify()
#include <iostream>	// for std::cout
#include <thread>	// for std::this_thread
#include <vector>	// for std::vector
#include <future>	// for std::future
#include <algorithm>

/*
 * This is an example of C++11 async tasks.
 * 
 * References:
 * http://bartoszmilewski.com/2011/10/10/async-tasks-in-c11-not-quite-there-yet
 *
 * EXTRA_COMPILE_FLAGS=-std=c++11 -pthread
 * EXTRA_LINK_FLAGS=-std=c++11 -pthread -Wl,--no-as-needed
 */

int main(int argc, char** argv, char** envp) {
	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
	std::vector<std::future<void>> futures;
	const unsigned int task_num=20;
	for (unsigned int i = 0; i < task_num; ++i) {
		futures.push_back(std::async([i] {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << i << ": " << std::this_thread::get_id() << " " << std::endl;
		}));
	}
	for(auto& fut:futures) {
		fut.get();
	}
}
