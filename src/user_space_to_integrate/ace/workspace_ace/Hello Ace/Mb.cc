#include <ace/OS_NS_stdio.h> // This is for snprintf
#include <ace/Log_Msg.h> // This is for ACE_DEBUG
#include <ace/Message_Block.h> // This is for ACE_Message_Block

int ACE_TMAIN(int,ACE_TCHAR **)
{
        ACE_Message_Block *mb;
        const int size=128;
        // Notice that we define a constant size block
        ACE_NEW_RETURN(mb,ACE_Message_Block(size),-1);
        mb->
