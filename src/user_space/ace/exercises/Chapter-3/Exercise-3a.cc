#include <ace/config-lite.h>
#include <ace/OS_NS_unistd.h>
#include <ace/Activation_Queue.h>
#include <ace/Method_Request.h>
#include <ace/Task.h>
#include <ace/Future.h>
#include <ace/Auto_Ptr.h>
#include <ace/OS_NS_stdio.h>
#include <ace/Malloc_Base.h>
#include <ace/Message_Queue.h>
#include <ace/Read_Buffer.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class HA_ControllerAgent {
	// Proxy to the HA_Controller that is on the network.
public:
	HA_ControllerAgent() {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgent::HA_ControllerAgent"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Entered HA_ControllerAgent\n")));                                                                                                                                                                                                         // -- 5
		status_result_ = 1;
	}


	int status_update(void) {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgent::status_update"));
		ACE_DEBUG((LM_DEBUG,
		           ACE_TEXT("Obtaining a status_update in %t ")
		           ACE_TEXT("thread of control\n")));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             // -- 11
		// Simulate time to send message and get status.
		ACE_OS::sleep(2);
		return(next_result_id());
	}


private:
	int next_result_id(void) {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgent::next_cmd_id"));
		return(status_result_++);
	}


	int status_result_;
};

class StatusUpdate : public ACE_Method_Request {
public:
	StatusUpdate(HA_ControllerAgent & controller, ACE_Future<int> &returnVal)
	: controller_(controller), returnVal_(returnVal) {
		ACE_TRACE(ACE_TEXT("StatusUpdate::StatusUpdate"));
	}


	virtual int call(void) {
		ACE_TRACE(ACE_TEXT("StatusUpdate::call"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Entered call\n")));                                                                                                                                                                                                         // -- 10

		// status_update with the controller.
		this->returnVal_.set(this->controller_.status_update());
		return(0);
	}


private:
	HA_ControllerAgent& controller_;
	ACE_Future<int>     returnVal_;
};

class ExitMethod : public ACE_Method_Request {
public:
	virtual int call(void) {
		// Cause exit.
		return(-1);
	}
};

class Scheduler : public ACE_Task_Base {
public:
	//
	// scheduler Function ------
	//
	Scheduler() {
		ACE_TRACE(ACE_TEXT("Scheduler::Scheduler"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Entered Scheduler Before this->activate()\n")));                                                                                                                                                                                                             // -- 2
		this->activate();
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Leaving Scheduler After this->activate()\n")));                                                                                                                                                                                                              // -- 4
	}


	virtual int svc(void) {
		ACE_TRACE(ACE_TEXT("Scheduler::svc"));

		while (1) {                                                                                                                                                                                                         // Dequeue the next method object
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("In Scheduler::svc waiting for queue data dequeue\n")));
			auto_ptr<ACE_Method_Request> request(this->activation_queue_.dequeue());

			// Invoke the method request.
			if (request->call() == -1) {
				break;
			}
		}

		return(0);
	}


	int enqueue(ACE_Method_Request *request) {
		ACE_TRACE(ACE_TEXT("Scheduler::enqueue"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("In Scheduler::enqueue\n")));                                                                                                                                                                                                         // -- 9
		return(this->activation_queue_.enqueue(request));
	}


private:
	ACE_Activation_Queue activation_queue_;
};

class HA_ControllerAgentProxy { // This acts as a Proxy to the controller impl object.
public:
	//
	// Future Function ------
	//
	ACE_Future<int> status_update(void) {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgentProxy::status_update"));
		ACE_Future<int> result;
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("In HA_ControllerAgentProxy::status_update\n")));                                                                                                                                                                                                         // -- 7

		// Create and enqueue a method request on the scheduler.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Activating enque\n")));                                                                                                                                                                                                         // -- 8
		this->scheduler_.enqueue(new StatusUpdate(this->controller_, result));

		// Return Future to the client.
		return(result);
	}


	//FUZZ: disable check_for_lack_ACE_OS
	void exit(void) {                                                                                                                                                                                                                                                   //FUZZ: enable check_for_lack_ACE_OS
		ACE_TRACE(ACE_TEXT("HA_ControllerAgentProxy::exit"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("In HA_ControllerAgentProxy::exit\n")));                                                                                                                                                                                                          // -- 18
		this->scheduler_.enqueue(new ExitMethod);
	}


private:
	Scheduler scheduler_;
	HA_ControllerAgent controller_;
};

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	ACE_Read_Buffer rb(ACE_STDIN);

	HA_ControllerAgentProxy controller;
	int counter = 0;

	ACE_Future<int> results[100];

	for (; ;) {
		char              *buffer = rb.read('\n');                                                                                                                                                                                                         // Allocate a new buffer.
		ACE_Message_Block *mb;

		if (buffer == 0) {
			break;
		}
		// Allocate a new message, but have it "borrow" its memory from the buffer.
		ACE_NEW_RETURN(mb, ACE_Message_Block(rb.size(), ACE_Message_Block::MB_DATA,
		                                     0, buffer), 0);
		// get message size
		//ACE_Utils::truncate_cast<unsigned long> (rb.size ());
		mb->wr_ptr(rb.size());

		results[counter] = controller.status_update();
		counter++;                                                                                                                                                                                                                      // We count the messages being sent

		ACE_DEBUG((LM_DEBUG, ACE_TEXT("counter=%d buffer=<%s>\n"), counter, buffer));
	}                                                                                                                  // End for loop

	counter--;
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("counter max=%d\n"), counter));
	ACE_OS::sleep(5);                                                                                                      // Do other work.
	return(0);

	// Get results...
	for (int j = 0; j < counter; j++) {
		int result = 0;
		results[j].get(result);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("New status_update %d\n"), result));                                                                                                                                                                                                            // -- 16
	}

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Calling the controller exit \n")));                                                                                                       // -- 17
	// Cause the status_updater threads to exit.
	controller.exit();
	ACE_Thread_Manager::instance()->wait();
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Done                     \n")));                                                                                                       // -- 18
	return(0);
}
