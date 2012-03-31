#include<ace/OS_main.h>
#include<ace/OS_Memory.h>
#include<ace/OS_NS_stdio.h>
#include<ace/OS_NS_string.h>
#include<ace/Log_Msg.h>
#include<ace/Message_Block.h>

/*
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int, ACE_TCHAR **) {
	ACE_Message_Block *mb;

	ACE_NEW_RETURN(mb, ACE_Message_Block(128), -1);

	const char *command1 = "<InitialDataPart>";
	const char *command2 = "< More Data ...>";

	// Use the pointer to insert the string
	ACE_OS::sprintf(mb->wr_ptr(), command1);
	// Move the wr_ptr() forward in the buffer by the amount of data we just put in.
	mb->wr_ptr(ACE_OS::strlen(command1));
	// Use the pointer to add another string
	ACE_OS::sprintf(mb->wr_ptr(), command2);
	mb->wr_ptr(ACE_OS::strlen(command2) + 1);

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Command Sequence --> %C\n"), mb->rd_ptr()));
	mb->rd_ptr(ACE_OS::strlen(mb->rd_ptr()) + 1);
	mb->release();
	// Send a hangup notification to the receiver.
	ACE_NEW_RETURN(mb, ACE_Message_Block(128, ACE_Message_Block::MB_HANGUP), -1);
	// Send an error notification to the receiver.
	mb->msg_type(ACE_Message_Block::MB_ERROR);
	mb->release();
	return(0);
}
