#include<ace/Log_Msg.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int, ACE_TCHAR *argv[]) {
	ACE_LOG_MSG->open(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT("syslogTest"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IEytan Carmiel was here\n")));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%IEytan Carmiel Can't be here\n")));
	ACE_LOG_MSG->open(argv[0]);
	ACE_DEBUG((LM_INFO, ACE_TEXT("argv[0]=%s\n"), argv[0]));
}
