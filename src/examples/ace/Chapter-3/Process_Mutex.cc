#include<ace/OS_NS_unistd.h>
#include<ace/Log_Msg.h>
#include<ace/Process.h>
#include<ace/Process_Mutex.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class GResourceUser {
public:
	GResourceUser(ACE_Process_Mutex & mutex) : gmutex_(mutex) {
		ACE_TRACE(ACE_TEXT("GResourceUser::GResourceUser"));
	}


	void run(void) {
		ACE_TRACE(ACE_TEXT("GResourceUser::run"));

		int count = 0;
		while(count++ < 5) {
			int result = this->gmutex_.acquire();
			ACE_ASSERT(result == 0);

			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P| %t) has the mutex (count=%d)\n"), count));

			// Access Global resource
			ACE_OS::sleep(1);

			result = this->gmutex_.release();
			ACE_ASSERT(result == 0);
			// Give other process a chance.
			ACE_OS::sleep(1);
		}
	}


private:
	ACE_Process_Mutex& gmutex_;
};

int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) {
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

	return(0);
}
