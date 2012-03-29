#define ACE_NTRACE 0
#include <ace/Log_Msg.h>

/*
 * This is a basic example of using ACE_DEBUG and ACE_TRACE.
 *
 * Notes:
 * - the ACE_NTRACE macro controls whether trace does anything special or
 * not.
 * - the "%I" ACE_DEBUG modifier adds indentation level which matches the trace
 * level.
 * - you have to put your own newline at the end of ACE_DEBUG messages.
 */

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
void foo(void) {
	ACE_TRACE(ACE_TEXT("foo"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHowdy Pardner\n")));
}

int ACE_TMAIN(int, ACE_TCHAR **) {
	ACE_TRACE(ACE_TEXT("main"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHi Mom\n")));
	foo();
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnight\n")));
	foo();
	return(0);
}
