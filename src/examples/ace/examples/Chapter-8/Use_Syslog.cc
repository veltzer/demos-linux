#include <ace/Log_Msg.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

void foo(void);

int ACE_TMAIN(int, ACE_TCHAR *argv[]) {
	// This will be directed to stderr (the default ACE_Log_Msg behavior).
	ACE_TRACE("main");

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%IHi Mom\n")));

	// Everything from foo() will be directed to the system logger
	// open: Program Name
	// ACE_Log_Msg::SYSLOG - Use system log file
	// text that will appear on each logged line
	ACE_LOG_MSG->open(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT("syslogTest"));
	foo();
	// Now we reset the log output to default (stderr)
	ACE_LOG_MSG->open(argv[0]);
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnight\n")));
	return(0);
}


void foo(void) {
	ACE_TRACE("foo");
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IThis is a log test\n")));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(0)Is this info seen?\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%I(1)Is this info seen?\n")));
}
