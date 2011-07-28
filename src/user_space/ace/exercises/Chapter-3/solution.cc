#define ACE_NTRACE 1
#include <ace/Task.h> // for ACE_Task_Base
#include <sched.h> // for sched_yield(2)
#include <assert.h> // for assert(3)

/*
 * This is a solution to the exercise.
 *
 * Try to run this with "taskset 1" and more to see the difference in performance.
 */

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
const int numAttempts=10000;
// should the threads yield one for the other ?
const bool yield=true;
// should we write debug messages ?
const bool debug=false;

class SharedResource {
	private:
		// This is the ACE mutex that wraps the OS mutex which we will
		// use to prevent simultaneous access to the resource.
		ACE_Thread_Mutex mutex;
		// This is the actual counter. We will initialize it in the constructor.
		int LockedCounter;
		// This is the attempt counter. We will initialize it in the constructor.
		int AttemptCounter;
		// Number of threads (to do the modulu...)
		int numThreads;
	public:
		SharedResource(int inumThreads) {
			LockedCounter = 0;
			AttemptCounter = 0;
			numThreads=inumThreads;
		}


		// No need for any locking for getting the value
		int getLockedCounter() {
			return(LockedCounter);
		}

		// No need for any locking for getting the value
		int getAttemptCounter() {
			return(AttemptCounter);
		}

		// This method only attempts an increase and returns whether the
		// attempt succeeded or not.
		bool attemptIncreaseValue(int value) {
			bool result = false;

			if(debug) {
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) waiting for lock\n")));
			}
			// wait until mutex is acquired
			mutex.acquire();
			AttemptCounter++;
			if(debug) {
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Trying to update variable modulu=%d value=%d\n"), LockedCounter % numThreads, value));
			}
			if (LockedCounter % numThreads == value) {
				if(debug) {
					ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) increasing counter\n")));
				}
				LockedCounter++;
				result = true;
			}
			mutex.release();
			if(debug) {
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) released lock\n")));
			}
			if(yield) {
				//ACE_OS::thr_yield();
				int ret=sched_yield();
				assert(ret!=-1);
			}
			return(result);
		}

		// This method will keep on trying until it succeeds. Watch out! Given
		// the right set of cirumstances this method will loop forever.
		void increaseValue(int value) {
			while (!attemptIncreaseValue(value)) {
			}
		}
};

class HA_CommandHandler : public ACE_Task_Base {
	private:
		SharedResource&  sharedResource;
		const int value;
	public:
		HA_CommandHandler(SharedResource & sharedResource, const int value) : sharedResource(sharedResource), value(value) {
		}

		// The real body of the thread
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) start\n")));
			for (int i = 0; i < numAttempts; i++) {
				sharedResource.attemptIncreaseValue(value);
				// Sleep for 1 sec to let the other thread try to aquire the lock
				// What if we remove this sleep period? Will the program work ?
				//ACE_OS::sleep(1);
			}
			return(0);
		}
};

int ACE_TMAIN(int argc, ACE_TCHAR ** argv) {
	int num_threads=3;
	if(argc>1) {
		num_threads=atoi(argv[1]);
	}
	SharedResource sharedResource(num_threads);

	HA_CommandHandler* handlers[num_threads];
	for(int i=0;i<num_threads;i++) {
		handlers[i]=new HA_CommandHandler(sharedResource, i);
	}
	for(int i=0;i<num_threads;i++) {
		handlers[i]->activate();
	}
	for(int i=0;i<num_threads;i++) {
		handlers[i]->wait();
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) LockedCounter=%d\n"), sharedResource.getLockedCounter()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) AttemptCounter=%d\n"), sharedResource.getAttemptCounter()));
	return(0);
}
