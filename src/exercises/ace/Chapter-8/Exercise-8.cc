#define ACE_NTRACE 1
#include<ace/streams.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Mark Veltzer
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
void FunB() {
	ACE_TRACE(ACE_TEXT("FunB"));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("DEBUG : pid=%P Line:%l File=%N\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("INFO : pid=%P Line:%l File=%N\n")));
	ACE_DEBUG((LM_NOTICE, ACE_TEXT("NOTICE: pid=%P Line:%l File=%N\n")));
}

void FunA() {
	ACE_TRACE(ACE_TEXT("FunA"));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("DEBUG : pid=%P Line:%l File=%N\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("INFO : pid=%P Line:%l File=%N\n")));
	ACE_DEBUG((LM_NOTICE, ACE_TEXT("NOTICE: pid=%P Line:%l File=%N\n")));
	FunB();
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv) {
	ACE_LOG_MSG->open(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT("ACE-Course"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%I a - Eytan Carmiel was here\n")));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I b - Eytan Carmiel Can't be here\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("argv[0]=%s\n"), argv[0]));
	ACE_DEBUG((LM_INFO, ACE_TEXT("INFO : pid=%P Line:%l File=%N\n")));
	ACE_LOG_MSG->priority_mask(LM_DEBUG | LM_INFO | LM_NOTICE | LM_TRACE, ACE_Log_Msg::PROCESS);
	ACE_TRACE(ACE_TEXT("main"));
	for(int i=0;i<2;i++) {
		FunA();
	}
	ACE_LOG_MSG->open(argv[0]);
	ACE_DEBUG((LM_INFO, ACE_TEXT("End of Program\n")));
	return EXIT_SUCCESS;
}
