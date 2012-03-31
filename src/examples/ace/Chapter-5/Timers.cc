#include<ace/config-all.h>
#include<ace/OS_main.h>
#include<ace/streams.h>
#include<ace/OS_NS_unistd.h>
#include<ace/OS_NS_time.h>
#include<ace/OS_NS_signal.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

typedef void (*timerTask_t)(void);

pid_t timerTask(int initialDelay, int interval, timerTask_t task) {
	if((initialDelay < 1) && (interval < 1)) {
		return(-1);
	}
	pid_t pid = ACE_OS::fork();
	if(pid < 0) {
		return(-1);
	}
	if(pid > 0) {
		return(pid);
	}
	if(initialDelay > 0) {
		ACE_OS::sleep(initialDelay);
	}
	if(interval < 1) {
		return(0);
	}
	while(true) {
		(*task)();
		ACE_OS::sleep(interval);
	}
	ACE_NOTREACHED(return 0);
}

void foo() {
	time_t now = ACE_OS::time(0);
	cerr << "The time is " << ACE_OS::ctime(&now) << endl;
}

void programMainLoop(void) {
	ACE_OS::sleep(30);
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv) {
	pid_t timerId = timerTask(3, 5, foo);
	programMainLoop();
	ACE_OS::kill(timerId, SIGINT);
	return EXIT_SUCCESS;
}
