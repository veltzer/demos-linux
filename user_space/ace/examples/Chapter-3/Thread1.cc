#include <ace/Synch.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

//Arguments that are to be passed to the worker thread are passed through this class.
class Args {
public:
	Args(int iterations) :
		mutex_(), iterations_(iterations) {
	}
	ACE_Thread_Mutex mutex_;
	int iterations_;
};

//The starting point for the worker threads
static void *
worker(void *arguments) {
	Args *arg = (Args *)arguments;

	for (int i = 0; i < arg->iterations_; i++) {
		ACE_DEBUG((LM_DEBUG,"(%t) Trying to get a hold of this iteration\n"));
		//This is our critical section
		arg->mutex_.acquire();
		ACE_DEBUG((LM_DEBUG, "(%t) This is iteration number %d\n", i));
		//work
		ACE_OS::sleep(2);
		arg->mutex_.release();
	}
	return(0);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		ACE_OS::printf("Usage: egx <number_of_threads> <number_of_iterations>\n");
		ACE_OS::exit(1);
	}
	//Setup the arguments
	Args arg(ACE_OS::atoi(argv[2]));

	ACE_Thread::spawn_n(ACE_OS::atoi(argv[1]), (ACE_THR_FUNC)worker, (void *)&arg);
	//Spawn the worker threads
	while (ACE_Thread::join(NULL, NULL, NULL) == 0) {
	}
}
