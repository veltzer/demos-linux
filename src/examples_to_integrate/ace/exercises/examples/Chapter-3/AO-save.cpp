#include<ace/config-lite.h>
#include<ace/OS_NS_unistd.h>
#include<ace/Activation_Queue.h>
#include<ace/Method_Request.h>
#include<ace/Task.h>
#include<ace/Future.h>
#include<ace/Auto_Ptr.h>

class HA_ControllerAgent
{
  // Proxy to the HA_Controller that is on the network.
public:
  HA_ControllerAgent ()
  { ACE_TRACE (ACE_TEXT ("HA_ControllerAgent::HA_ControllerAgent"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Entered HA_ControllerAgent\n")));
    status_result_ = 1;
  }

  int status_update (void)
  { ACE_TRACE (ACE_TEXT ("HA_ControllerAgent::status_update"));
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%N - Obtaining a status_update in %t ")
                ACE_TEXT ("thread of control\n")));
    // Simulate time to send message and get status.
    ACE_OS::sleep (2);
    return next_result_id ();
  }

private:
  int next_result_id (void)
  { ACE_TRACE (ACE_TEXT ("HA_ControllerAgent::next_cmd_id"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Entered next_result_id\n")));
    return status_result_++;
  }

  int status_result_;
};

class StatusUpdate : public ACE_Method_Request
{
public:
  StatusUpdate (HA_ControllerAgent& controller,
                ACE_Future<int>& returnVal)
    : controller_(controller), returnVal_(returnVal)
  {
    ACE_TRACE (ACE_TEXT ("StatusUpdate::StatusUpdate"));
  }

  virtual int call (void)
  { ACE_TRACE (ACE_TEXT ("StatusUpdate::call"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Entered call\n")));

    // status_update with the controller.
    this->returnVal_.set (this->controller_.status_update ());
    return 0;
  }

private:
  HA_ControllerAgent& controller_;
  ACE_Future<int> returnVal_;
};

class ExitMethod : public ACE_Method_Request
{
public:
  virtual int call (void)
  {
    // Cause exit.
    return -1;
  }
};

class Scheduler : public ACE_Task_Base
{
public:
	//
	// scheduler Function ------
	//
  Scheduler ()
  { ACE_TRACE (ACE_TEXT ("Scheduler::Scheduler"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Entered Scheduler Before this->activate()\n")));
    this->activate ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Leaving Scheduler After this->activate()\n")));
  }

  virtual int svc (void)
  { ACE_TRACE (ACE_TEXT ("Scheduler::svc"));

    while (1)
      { // Dequeue the next method object
        ACE_DEBUG ((LM_DEBUG,
				            ACE_TEXT ("%N - In Scheduler::svc waiting for queue data dequeue\n")));
        auto_ptr<ACE_Method_Request> request (this->activation_queue_.dequeue ());

        // Invoke the method request.
        if (request->call () == -1)
          break;
      }

    return 0;
  }

  int enqueue (ACE_Method_Request *request)
  { ACE_TRACE (ACE_TEXT ("Scheduler::enqueue"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - In Scheduler::enqueue\n")));
    return this->activation_queue_.enqueue (request);
  }

private:
  ACE_Activation_Queue activation_queue_;
};

class HA_ControllerAgentProxy
{ // This acts as a Proxy to the controller impl object.
public:
	//
	// Future Function ------
	//
  ACE_Future<int> status_update (void)
  { ACE_TRACE (ACE_TEXT ("HA_ControllerAgentProxy::status_update"));
    ACE_Future<int> result;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - In HA_ControllerAgentProxy::status_update\n")));

    // Create and enqueue a method request on the scheduler.
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Activating enque\n")));
    this->scheduler_.enqueue (new StatusUpdate (this->controller_, result));

    // Return Future to the client.
    return result;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void exit (void)
  { //FUZZ: enable check_for_lack_ACE_OS
    ACE_TRACE (ACE_TEXT ("HA_ControllerAgentProxy::exit"));
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - In HA_ControllerAgentProxy::exit\n")));
    this->scheduler_.enqueue (new ExitMethod);
  }

private:
  Scheduler scheduler_;
  HA_ControllerAgent controller_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{ 

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Constructing HA_ControllerAgentProxy controller\n")));
  HA_ControllerAgentProxy controller;
  ACE_Future<int> results[10];

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Activating the controller.status_update\n")));
  for (int i = 0 ; i < 1; i++)
    results[i] = controller.status_update ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - We simulate doing other work by sleep(5)\n")));
  ACE_OS::sleep (5);  // Do other work.

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - ------Until now we did not request any data----\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - After the sleep we are going to get the results\n")));

  // Get results...
  for (int j = 0; j < 1; j++)
    { int result = 0;
      results[j].get (result);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - New status_update %d\n"), result));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%N - Calling the controller exit \n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("----------------------------\n")));
  // Cause the status_updater threads to exit.
  controller.exit ();
  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("-------------------------\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Done                     \n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("-------------------------\n")));
  return 0;
}
