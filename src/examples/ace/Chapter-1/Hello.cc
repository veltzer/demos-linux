#include<ace/Log_Msg.h> // for ACE_DEBUG and ACE_ERROR
#include<ace/OS_NS_stdio.h> // for the printf function
#include<ace/streams.h> // for C++ streams interface
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Short program demonstrates printing Hello World using the ACE environment
 * notice that both ACE_DEBUG and ACE_ERROR are going to stderr.
 *
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

// Notice the use of the ACE version of main
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	printf("It seems that printf is working\n");
	cout << "It seems like C++ streams are working without std:: prefix" << endl;
	// Notice that ACE_DEBUG takes one argument
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("ACE_DEBUG(pid=%P Time:%T)\n")));
	ACE_ERROR((LM_ERROR, ACE_TEXT("ACE_ERROR(pid=%P File:%N Line:%l Date+Time:%D)\n")));
	return EXIT_SUCCESS;
}
