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
#include <ace/Task.h>	// for ACE_Task_Base
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a simple ACE_Task_Base showing how to create your own threads
 * in an OO fashion.
 *
 * Coding the thread:
 * - inherit from ACE_Task_Base
 * - create constructor for neccessary initialisation values.
 * - create methods that return the final return value.
 * - code the svc method. (what about it's return value?!?).
 *
 * Running the thread:
 * - create an instance of your class (supply neccessary arguments to constructor).
 * - .activate() to run the thread (returns error code).
 * - .wait() to wait for it to be over (returns error code).
 * - get your return values
 *
 * EXTRA_COMPILE_CMD=pkg-config --cflags ACE
 * EXTRA_LINK_CMD=pkg-config --libs ACE
 */

class MyThread : public ACE_Task_Base {
private:
	int val;

public:
	MyThread(int ival) : val(ival) {
	}
	int getVal() {
		return val;
	}
	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) thread: starting\n")));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) thread: ending\n")));
		return 0;
	}
};

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: starting\n")));
	MyThread thread(7);
	thread.activate();
	thread.wait();
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: res is %d\n"), thread.getVal()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) main: ending\n")));
	return EXIT_SUCCESS;
}
