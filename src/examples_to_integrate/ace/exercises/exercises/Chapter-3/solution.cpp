#define ACE_TRACE 0

#include<ace/Task.h>

class SharedResource 
{
	private:
		// This is the ACE mutex that wraps the OS mutex which we will 
		// use to prevent simultaneous access to the resource.
		ACE_Thread_Mutex mutex;
		// This is the actual counter. We will initialize it in the constructor.
		int LockedCounter;
		// This is the attempt counter. We will initialize it in the constructor.
		int AttemptCounter;
	public:
		SharedResource() {
			LockedCounter=0;
			AttemptCounter=0;
		}

		// No need for any locking for getting the value
		int getLockedCounter() {
			return LockedCounter;
		}
		// No need for any locking for getting the value
		int getAttemptCounter() {
			return AttemptCounter;
		}

		// This method only attempts an increase and returns whether the
		// attempt succeeded or not.
		bool attemptIncreaseValue(int value)
		{
			bool result=false;
			// wait until mutex is acquired
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) waiting for lock\n")));
			mutex.acquire();
			AttemptCounter++;;
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) Trying to update variable modulu=%d value=%d\n"),
				LockedCounter%3,value));
			if(LockedCounter%3==value) {
				ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) increasing counter\n")));
				LockedCounter++;
				result=true;
			}
			mutex.release();
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) released lock\n")));
			return result;
		}
		// This method will keep on trying until it succeeds. Watch out! Given
		// the right set of cirumstances this method will loop forever.
		void increaseValue(int value) {
			while(!attemptIncreaseValue(value)) {};
		}
};

class HA_CommandHandler:public ACE_Task_Base
{
	private:
		static const int numAttempts=25;
		SharedResource& sharedResource;
		const int value;

	public:
		HA_CommandHandler(SharedResource& sharedResource,const int value):sharedResource(sharedResource),value(value) {}

		// The real body of the thread
		virtual int svc(void)
		{
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) Handler Thread running\n")));
			for (int i=0;i<numAttempts;i++) {
			//while(LockedCounter<25)
				sharedResource.increaseValue(value);
				// Sleep for 1 sec to let the other thread try to aquire the lock
				// What if we remove this sleep period? Will the program work ?
				//ACE_OS::sleep(1);
			}
			return 0;
		}
};

int ACE_TMAIN (int,ACE_TCHAR *[]) {
	SharedResource sharedResource;
	HA_CommandHandler handler1(sharedResource,0);
	HA_CommandHandler handler2(sharedResource,1);
	HA_CommandHandler handler3(sharedResource,2);
	handler1.activate();
	handler2.activate();
	handler3.activate();
	handler1.wait();
	handler2.wait();
	handler3.wait();
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) LockedCounter=%d\n"),sharedResource.getLockedCounter()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("(%t) GeneralCounter=%d\n"),sharedResource.getAttemptCounter()));
	return 0;
}
