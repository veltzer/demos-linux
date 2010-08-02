/*
 * This demo shows the ACE logging facilities
 *
 * ACE_NTRACE: supports whether ACE_TRACE actually does anything. If disabled then ACE_TRACE
 * is nothing. If enabled then:
 * - traces will be printed where the are in the code (ACE_TRACE).
 * - traces will be printed on exit from functions (destructor pattern).
 * - %I can be used in ACE_DEBUG in order to indent messages properly.
 * - functions that do not use ACE_TRACE in them will not show up or contribute
 *   to the trace nesting at all.
 *
 * note that ACE_NTRACE can be enabled/disabled on a per file basis and so can
 * only be enabled at compile time in modules that you want to debug...
 */

// the next
#define ACE_NTRACE 1
#include <ace/Log_Msg.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
void bar(void) {
	ACE_TRACE(ACE_TEXT("bar"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IMessage from bar\n")));
}


void foo(void) {
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IMessage from foo\n")));
	bar();
}


int ACE_TMAIN(int, ACE_TCHAR **) {
	ACE_TRACE(ACE_TEXT("main"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IBefore\n")));
	foo();
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IAfter\n")));
	return(0);
}
