#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Notes:
 * - note the care which is taken to keep the compiler from
 * inlining the foo function.
 *
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

void __attribute__((noinline)) foo(void) {
	ACE_TRACE("foo");
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IThis is a log test\n")));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(0)Is this info seen?\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%I(1)Is this info seen?\n")));
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
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
	return EXIT_SUCCESS;
}

