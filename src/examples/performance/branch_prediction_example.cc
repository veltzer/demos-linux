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
#include <stdlib.h>	// for EXIT_SUCCESS, rand(3)
#include <iostream>	// for std::cout, std::endl
#include <algorithm>	// for std::rand(), std::sort()
#include <time.h>	// for clock(3), clock_t:type, CLOCKS_PER_SEC

/*
 * This is an example which shows a real difference in performance when
 * having and not having correct branch prediction by the CPU.
 *
 * References:
 * http://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-an-unsorted-array
 */

void do_work(int* data, unsigned int arraySize, const char* msg) {
	clock_t start = clock();
	
	long long sum = 0;
	for (unsigned i = 0; i < 100000; ++i) {
		// Primary loop
		for (unsigned c = 0; c < arraySize; ++c) {
			if (data[c] >= 128)
				sum += data[c];
		}
	}

	clock_t end = clock();
	double elapsedTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	std::cout << msg << std::endl;
	std::cout << elapsedTime << std::endl;
	std::cout << "sum = " << sum << std::endl;
}

int main(int argc, char** argv, char** envp) {
	// Generate data
	const unsigned int arraySize = 32768;
	int data[arraySize];
	for (unsigned c = 0; c < arraySize; ++c)
		data[c] = std::rand() % 256;

	// run unsorted and then sorted
	do_work(data, arraySize, "unsorted");
	std::sort(data, data + arraySize);
	do_work(data, arraySize, "sorted");
	return EXIT_SUCCESS;
}
