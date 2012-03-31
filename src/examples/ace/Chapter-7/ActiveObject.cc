#include<ace/config-lite.h>
#include<ace/OS_NS_unistd.h>
#include<ace/Activation_Queue.h>
#include<ace/Method_Request.h>
#include<ace/Task.h>
#include<ace/Future.h>
#include<ace/Auto_Ptr.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class HA_ControllerAgent { // Proxy to the HA_Controller that is on the network.
public:
	HA_ControllerAgent() {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgent::HA_ControllerAgent"));
		status_result_ = 1;
	}


	int status_update(void) {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgent::status_update"));
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Obtaining a status_update in %t ") ACE_TEXT("thread of control\n")));
		// Simulate sending message to controller and getting status.
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
	StatusUpdate(HA_ControllerAgent & controller, ACE_Future<int> &returnVal) : controller_(controller), returnVal_(returnVal) {
		ACE_TRACE(ACE_TEXT("StatusUpdate::StatusUpdate"));
	}


	virtual int call(void) {
		ACE_TRACE(ACE_TEXT("StatusUpdate::call"));

		// status_update with the controller.
		this->returnVal_.set(this->controller_.status_update());
		return(0);
	}


private:
	HA_ControllerAgent& controller_;
	ACE_Future<int> returnVal_;
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
	Scheduler() {
		ACE_TRACE(ACE_TEXT("Scheduler::Scheduler"));
		this->activate();
	}

	virtual int svc(void) {
		ACE_TRACE(ACE_TEXT("Scheduler::svc"));
		while(true) {
			// Dequeue the next method object
			auto_ptr<ACE_Method_Request>
			request(this->activation_queue_.dequeue());

			// Invoke the method request.
			if(request->call() == -1) {
				break;
			}
		}
		return(0);
	}

	int enqueue(ACE_Method_Request *request) {
		ACE_TRACE(ACE_TEXT("Scheduler::enqueue"));
		return(this->activation_queue_.enqueue(request));
	}

private:
	ACE_Activation_Queue activation_queue_;
};

class HA_ControllerAgentProxy { // This acts as a Proxy to the controller impl object.
public:
	ACE_Future<int> status_update(void) {
		ACE_TRACE(ACE_TEXT("HA_ControllerAgentProxy::status_update"));
		ACE_Future<int> result;

		// Create and enqueue a method request on the scheduler.
		this->scheduler_.enqueue(new StatusUpdate(this->controller_, result));

		// Return Future to the client.
		return(result);
	}


	//FUZZ: disable check_for_lack_ACE_OS
	void exit(void) {
		//FUZZ: enable check_for_lack_ACE_OS
		ACE_TRACE(ACE_TEXT("HA_ControllerAgentProxy::exit"));
		this->scheduler_.enqueue(new ExitMethod);
	}


private:
	Scheduler scheduler_;
	HA_ControllerAgent controller_;
};

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	HA_ControllerAgentProxy controller;

	ACE_Future<int> results[10];

	for(int i=0;i<10;i++) {
		results[i] = controller.status_update();
	}

	ACE_OS::sleep(5);
	// Do other work.

	// Get results...
	for(int j=0;j<10;j++) {
		int result=0;
		results[j].get(result);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("New status_update %d\n"), result));
	}

	// Cause the status_updater threads to exit.
	controller.exit();
	ACE_Thread_Manager::instance()->wait();
	return(0);
}
