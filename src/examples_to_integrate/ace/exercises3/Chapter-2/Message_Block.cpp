#include<ace/OS_NS_stdio.h> // This is for snprintf
#include<ace/Log_Msg.h> // This is for ACE_DEBUG
#include<ace/Message_Block.h> // This is for ACE_Message_Block

/*
 * This demo shows how to create message blocks, how to set message types, how to create
 * error or hangup message, how to populate message blocks in static or dynamic fashion
 *
 * This demo does not explain what ACE_NEW_RETURN is. See slides or a different example
 * for that...
 */

int ACE_TMAIN(int,ACE_TCHAR **)
{
	// This sections explains how to create a message block with some data in it
	// It shows how to use the ACE_NEW_RETURN construct
	// How to initialize a message block with constant size and how to write
	// data to the message block
	ACE_Message_Block *mb;
	const int size=128;
	// Notice that we define a constant size block
	ACE_NEW_RETURN(mb,ACE_Message_Block(size),-1);
	const char *data1="This is some data.";
	const char *data2="This is even more Data...";
	// We have a 128 bytes at mb->wr_ptr so lets write something on them 
	// mb->wr_ptr also moves the pointer forward
	mb->wr_ptr(ACE_OS::snprintf(mb->wr_ptr(),size,data1));
	mb->wr_ptr(ACE_OS::snprintf(mb->wr_ptr(),size,data2));
	// Lets access the buffer in read only mode
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// This is to show that the rd_ptr does not change across calls
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// This is to show that we can set the read pointer to some place
	// and read from there
	// lets do it using a pointer (absolutely)
	mb->rd_ptr(mb->rd_ptr()+6);
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// lets do it using an integer (relativly)
	mb->rd_ptr(-2);
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// again, rd_ptr does not change
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// Lets print the messages type. This is the default one
	// Lets also print some more types to compare
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Messages type is --> %d\n"),mb->msg_type()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("MB_HANGUP is --> %d\n"),ACE_Message_Block::MB_HANGUP));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("MB_ERROR is --> %d\n"),ACE_Message_Block::MB_ERROR));
	// Lets set the message blocks type to something and show it
	mb->msg_type(7);
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Messages type is --> %d\n"),mb->msg_type()));
	// Lets print the message size and the message length
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("size is --> %d\n"),mb->size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("length is --> %d\n"),mb->length()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_size is --> %d\n"),mb->total_size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_length is --> %d\n"),mb->total_length()));
	// Lets show how the dump method work (does not do anything by default)
	mb->dump();

	// finally, lets get ridd of the message block
	mb->release();

	// Send a hangup notification to the receiver. Notice the type parameter
	ACE_NEW_RETURN(mb,ACE_Message_Block(size,ACE_Message_Block::MB_HANGUP),-1);
	// Lets print the Message_Block's types
	if(mb->msg_type()==ACE_Message_Block::MB_HANGUP) {
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("We got a hangup message\n")));
	}
	// again, release the mb
	mb->release();

	// Send an error notification to the receiver. Notice the type parameter
	ACE_NEW_RETURN(mb,ACE_Message_Block(size,ACE_Message_Block::MB_ERROR),-1);
	if(mb->msg_type()==ACE_Message_Block::MB_ERROR) {
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("We got an error message\n")));
	}
	// again, release the mb
	mb->release();

	// Create a non constant size Message_Block...
	ACE_NEW_RETURN(mb,ACE_Message_Block(),-1);
	// Lets allocate message block size and write some data
	mb->size(size);
	mb->wr_ptr(ACE_OS::snprintf(mb->wr_ptr(),size,data1));
	// Lets print the message size and the message length
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("size is --> %d\n"),mb->size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("length is --> %d\n"),mb->length()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_size is --> %d\n"),mb->total_size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_length is --> %d\n"),mb->total_length()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	// Lets re-allocate more space and print again
	mb->size(size*2);
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("size is --> %d\n"),mb->size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("length is --> %d\n"),mb->length()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_size is --> %d\n"),mb->total_size()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("total_length is --> %d\n"),mb->total_length()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));

	// Lets start playing around with reference counting
	ACE_NEW_RETURN(mb,ACE_Message_Block(size),-1);
	mb->wr_ptr(ACE_OS::snprintf(mb->wr_ptr(),size,data1));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("reference_count is --> %d\n"),mb->reference_count()));
	ACE_Message_Block *mb2=mb->duplicate();
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("reference_count is --> %d\n"),mb->reference_count()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("reference_count is --> %d\n"),mb2->reference_count()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb->rd_ptr()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb2->rd_ptr()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Read pointer --> %x\n"),mb->rd_ptr()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Read pointer --> %x\n"),mb2->rd_ptr()));
	mb->release();
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("reference_count is --> %d\n"),mb2->reference_count()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Message data --> %C\n"),mb2->rd_ptr()));
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("Read pointer --> %x\n"),mb2->rd_ptr()));
	// lets release the memory ourselves (this is a mistake!!!)
	//char* ptr=mb2->rd_ptr();
	//ACE_Allocator::instance()->free (mb2->rd_ptr());
	// lets release the message block
	mb2->release();
	//ACE_Allocator::instance()->free (ptr);
	return 0;
}
