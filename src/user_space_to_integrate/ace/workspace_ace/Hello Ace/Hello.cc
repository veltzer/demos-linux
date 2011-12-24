// Short program demonstrates printing Hello World using the ACE environment
// notice that both ACE_DEBUG and ACE_ERROR are going to stderr.
#include <ace/Log_Msg.h> // for ACE_DEBUG and ACE_ERROR
#include <ace/OS_NS_stdio.h> // for the printf function
#include <ace/streams.h> // for C++ streams interface

// Notice the use of ACE_TMAIN for main
int ACE_TMAIN(int argc,ACE_TCHAR**) {
        printf("It seems that printf is working\n");
        cout << "It seems like C++ streams are working" << endl;
        // Notice that ACE_DEBUG takes one argument
        ACE_DEBUG((LM_DEBUG,ACE_TEXT("ACE_DEBUG(pid=%P Time:%T)\n")));
        ACE_ERROR((LM_ERROR,ACE_TEXT("ACE_ERROR(pid=%P File:%N Line:%l Date+Time:%D)\n")));
        return 0;
}
