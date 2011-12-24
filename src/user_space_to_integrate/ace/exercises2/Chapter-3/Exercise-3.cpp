#define ACE_TRACE 0

#include "ace/Task.h"

int LockingCounter  = 0;
int GeneralCounter = 0;
class HA_Device_Repository
{
public:
  HA_Device_Repository ()
  { }

  void update_variable (int value)
  { mutex_.acquire ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Updating variable modulu=%d value=%d\n"),
		             LockingCounter % 3, value ));
    if ( LockingCounter % 3  == value )
		     LockingCounter++;
    GeneralCounter++;;
//  ACE_OS::sleep (1);
    mutex_.release ();
    ACE_OS::sleep (1);  // Let the other thread to make the aquire
  }


private:
  ACE_Thread_Mutex mutex_;
};

class HA_CommandHandler : public ACE_Task_Base
{
public:
  enum {NUM_USES = 10};

  HA_CommandHandler (HA_Device_Repository& rep) : rep_(rep)
  { }

  void set_value (int value)
	 { value_ = value;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Setting value %d\n"), value_));
	 }


  virtual int svc (void)  // The real body of the thread
  { ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Handler Thread running\n")));
//  for (int i = 0; i < NUM_USES; i++)
    while ( LockingCounter < 25 )
      this->rep_.update_variable (value_);
    return 0;
  }

private:
  HA_Device_Repository & rep_;
	int value_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{ HA_Device_Repository rep;
  HA_CommandHandler handler1 (rep);
  HA_CommandHandler handler2 (rep);
  HA_CommandHandler handler3 (rep);
  handler1.set_value (2);
  handler2.set_value (1);
  handler3.set_value (0);
  handler1.activate ();
  handler2.activate ();
  handler3.activate ();

  handler1.wait ();
  handler2.wait ();
  handler3.wait ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) LockingCounter=%d\n"), LockingCounter));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) GeneralCounter=%d\n"), GeneralCounter));
  return 0;
}
