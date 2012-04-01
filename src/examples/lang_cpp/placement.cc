/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<stdlib.h>
#include<iostream>

#include<us_helper.h>

/*
 *	This example shows how to use the C++ operator new placement
 *	operator.
 *
 *	Things we learn:
 *	1. How to write your own placement function.
 *	2. Regular constructor gets called after the placement.
 *	3. Releasing of space could be overridden too.
 *	4. This could be used for caching and real time considerations for instance.
 *	5. Even if you allocate an array the delete[] is NOT called so
 *		your regular delete operator needs to know how to do the job
 *		both for arrays and for single elements (if you want arrays
 *		at all that is...).
 */

class A {
public:
	float val;
	A(void) {
		val = -7.6;
	}


	A(double ival) {
		val = ival;
	}


	void *operator new(size_t size, double val) {
		std::cerr << "in new operator" << std::endl;
		std::cerr << "size is " << size << std::endl;
		void *pointer = malloc(size);
		std::cerr << "pointer is " << pointer << std::endl;
		A *p = (A *)pointer;
		p->val = val;
		return(pointer);
	}


	// this is for allocating arrays, the size that you get
	// is SizeOfObject*NumOfObjects...
	void *operator new[] (const unsigned int size) {
		std::cerr << "in new[] operator" << std::endl;
		std::cerr << "size is " << size << std::endl;
		void *pointer = malloc(size);
		std::cerr << "pointer is " << pointer << std::endl;
		return(pointer);
	}


	// notice that this does NOT get called...
	void operator delete[] (void *pointer) {
		std::cerr << "in delete[] operator" << std::endl;
		std::cerr << "pointer is " << pointer << std::endl;
		free(pointer);
	}


	void *operator new(size_t size) {
		std::cerr << "in new operator" << std::endl;
		std::cerr << "size is " << size << std::endl;
		void *pointer = malloc(size);
		std::cerr << "pointer is " << pointer << std::endl;
		return(pointer);
	}


	void operator delete(void *pointer) {
		std::cerr << "in delete operator" << std::endl;
		std::cerr << "pointer is " << pointer << std::endl;
		free(pointer);
	}
};

int main(int argc, char **argv, char **envp) {
	DEBUG("heap no arguments example");
	A* a=new A();

	std::cerr << "a->val is " << a->val << std::endl;
	delete a;

	DEBUG("heap arguments example");
	A* b=new(5.5) A();
	std::cerr << "b->val is " << b->val << std::endl;
	delete b;

	DEBUG("many heap no arguments example");
	const unsigned int num_objs = 5;
	A* e=new A[num_objs];
	for(unsigned int i=0;i<num_objs;i++) {
		std::cerr << i << " " << "e->val is " << e[i].val << std::endl;
	}
	delete e;

	// the next two examples are stack examples in which case neither
	// the new nor the delete operator will be called (memory is stack
	// memory).
	// could you write a C++ object which can be used ONLY on the stack
	// or conversly ONLY on the heap using this property ?!?

	DEBUG("stack no arguments example");
	A c;
	std::cerr << "c.val is " << c.val << std::endl;

	DEBUG("stack arguments example");
	A d(6.7);

	std::cerr << "d.val is " << d.val << std::endl;
	return EXIT_SUCCESS;
}
