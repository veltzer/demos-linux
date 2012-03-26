#include <ace/Thread.h>
#include <ace/Synch.h>
#include <ace/Log_Msg.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

static int number = 0;
static int seed = 0;
static void * worker(void *arg) {
	ACE_UNUSED_ARG(arg);
	ACE_DEBUG((LM_DEBUG, "Thread (%t) Created to do some work"));
	::number++;
	ACE_DEBUG((LM_DEBUG, " and number is %d\n", ::number));
//Let the other guy go while I fall asleep for a random period of time
	ACE_OS::sleep(ACE_OS::rand() % 2);
//Exiting now
	ACE_DEBUG((LM_DEBUG, "\t\t Thread (%t) Done! \t The number is now: %d\n", number));
	return(0);
}

/*
class M {
	public:
		void run() {
			ACE_DEBUG((LM_DEBUG, "Thread (%t) Created to do some work"));
			::number++;
			ACE_DEBUG((LM_DEBUG, " and number is %d\n", ::number));
			//Let the other guy go while I fall asleep for a random period of time
			ACE_OS::sleep(ACE_OS::rand() % 2);
//Exiting now
			ACE_DEBUG((LM_DEBUG, "\t\t Thread (%t) Done! \t The number is now: %d\n", number));
		}
};

static void* run_method(void* arg) {
	M* p=(M*)arg;
	p->run();
	return NULL;
}
*/

int main(int argc, char** argv,char** envp) {
	if (argc < 2) {
		ACE_DEBUG((LM_DEBUG, "Usage: %s <number of threads>\n", argv[0]));
		ACE_OS::exit(1);
	}
	//setup the random number generator
	ACE_OS::srand(::seed);
	//number of threads to spawn
	unsigned int n_threads = ACE_OS::atoi(argv[1]);
	ACE_thread_t* threadID = new ACE_thread_t[n_threads + 1];
	ACE_hthread_t* threadHandles = new ACE_hthread_t[n_threads + 1];
	//spawn n_threads
	if (ACE_Thread::spawn_n(
			threadID, //id's for each of the threads
			n_threads, //number of threads to spawn
			(ACE_THR_FUNC)worker, //entry point for new thread
			NULL, //args to worker
			THR_JOINABLE | THR_NEW_LWP, //flags
			ACE_DEFAULT_THREAD_PRIORITY,
			NULL,
			0,
		threadHandles) != n_threads) {
		ACE_DEBUG((LM_DEBUG, "Error in spawning thread\n"));
	}
	//Wait for all the threads to exit before you let the main fall
	//through and have the process exit.
	for (unsigned int i = 0; i < n_threads; i++) {
		ACE_Thread::join(threadHandles[i]);
	}
	return(0);
}
