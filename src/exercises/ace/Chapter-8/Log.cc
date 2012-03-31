#include<ace/Log_Msg.h> // for ACE_DEBUG, ACE_LOG_MSG
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Mark Veltzer
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_LOG_MSG->open(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT("syslogTest"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%I 1-Eytan Carmiel was here\n")));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I 2-Eytan Carmiel Can't be here\n")));
	ACE_LOG_MSG->open(argv[0]);
	ACE_DEBUG((LM_INFO, ACE_TEXT("argv[0]=%s\n"), argv[0]));
	return EXIT_SUCCESS;
}
