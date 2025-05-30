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
#include <ace/OS_NS_unistd.h>
#include <ace/Log_Msg.h>
#include <ace/Process.h>
#include <ace/Process_Mutex.h>

/*
 * EXTRA_COMPILE_CMD=pkg-config --cflags ACE
 * EXTRA_LINK_CMD=pkg-config --libs ACE
 */

// Listing 1 code/ch10
class GResourceUser{
public:
	GResourceUser(ACE_Process_Mutex & mutex) : gmutex_(mutex) {
		ACE_TRACE("GResourceUser::GResourceUser");
	}

	void run(void) {
		ACE_TRACE("GResourceUser::run");

		int count=0;
		while(count++ < 10) {
			int result=this->gmutex_.acquire();
			ACE_ASSERT(result==0);

			ACE_DEBUG((LM_DEBUG, "(%P| %t) has the mutex\n"));

			// Access Global resource
			ACE_OS::sleep(1);

			result=this->gmutex_.release();
			ACE_ASSERT(result==0);
			// Give other process a chance.
			ACE_OS::sleep(1);
		}
	}

private:
	ACE_Process_Mutex& gmutex_;
};
// Listing 1

// Listing 0 code/ch10
int main(int argc, char** argv) {
	if (argc > 1) {
		// Run as the child.
		// Create or get the global mutex.
		ACE_Process_Mutex mutex("GlobalMutex");

		GResourceUser acquirer(mutex);

		acquirer.run();
	} else {
		// Run as the parent.
		ACE_Process_Options options;
		options.command_line("%s a", argv[0]);
		ACE_Process processa, processb;

		pid_t pida=processa.spawn(options);
		pid_t pidb=processb.spawn(options);

		ACE_DEBUG((LM_DEBUG, "Spawned processes; pids %d:%d\n", pida, pidb));
		if (processa.wait()==-1) {
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "processa wait"), -1);
		}
		if (processb.wait()==-1) {
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "processb wait"), -1);
		}
	}
	return 0;
}

// Listing 0
