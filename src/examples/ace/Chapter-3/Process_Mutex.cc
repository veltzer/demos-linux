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

#include<firstinclude.h>
#include<ace/OS_NS_unistd.h>
#include<ace/Log_Msg.h>
#include<ace/Process.h>
#include<ace/Process_Mutex.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class GResourceUser {
	public:
		GResourceUser(ACE_Process_Mutex & mutex):gmutex_(mutex) {
			ACE_TRACE(ACE_TEXT("GResourceUser::GResourceUser"));
		}
		void run(void) {
			ACE_TRACE(ACE_TEXT("GResourceUser::run"));
			int count=0;
			while(count++<5) {
				int result=this->gmutex_.acquire();
				ACE_ASSERT(result==0);
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P| %t) has the mutex (count=%d)\n"), count));
				// Access Global resource
				ACE_OS::sleep(1);
				result = this->gmutex_.release();
				ACE_ASSERT(result==0);
				// Give other process a chance.
				ACE_OS::sleep(1);
			}
		}
	private:
		ACE_Process_Mutex& gmutex_;
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	// Run as the child.
	if (argc > 1) {
		// Create or get the global mutex.
		ACE_Process_Mutex mutex("GlobalMutex");

		GResourceUser acquirer(mutex);

		acquirer.run();
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P| %t) Done (Child).\n")));
	} else {
		// Run as the parent.
		ACE_Process_Options options;
		options.command_line(ACE_TEXT("%s a"), argv[0]);
		ACE_Process processa, processb;

		pid_t pida = processa.spawn(options);
		pid_t pidb = processb.spawn(options);

		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Spawned processes; pids %d:%d\n"), pida, pidb));

		if (processa.wait() == -1) {
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("processa wait")), -1);
		}

		if (processb.wait() == -1) {
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("processb wait")), -1);
		}
		// Print Done for the parent only
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P| %t) Done (Parent).\n")));
	}
	return EXIT_SUCCESS;
}
