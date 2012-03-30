// Exercise 1
// 1. Write a program that will have two functions FunA and FunB.
//    use a loop counter to call Twice FunA and this function will call FunB.
// 2. Each function will print a message when entered, using the ACE utility.
// 3. Use the ACE to set and unset this capability.
// 4. In each function print debugging information using 3 levels:
//     1. LM_TRACE
//     2. LM_DEBUG
//     3. LM_INFO
//    Use the following options in each output line:
//     1. Print program pid.
//     2. Print line number.
//     3. Print File Name.
//
//    Try to verify what causes indentation.
//    %I - without trace nothing.
//    %I - with    trace about 12 characters. Multiple "%I " is possible
// 
#define ACE_NTRACE 0
#include<ace/streams.h>
#include<ace/Log_Msg.h>
void FunB ()
{ ACE_TRACE(ACE_TEXT ("FunB"));
  ACE_DEBUG ((LM_DEBUG , ACE_TEXT ("DEBUG : pid=%P Line:%l File=%N\n")));
  ACE_DEBUG ((LM_INFO  , ACE_TEXT ("INFO  : pid=%P Line:%l File=%N\n")));
  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("NOTICE: pid=%P Line:%l File=%N\n")));
  return;
}

void FunA ()
{ ACE_TRACE(ACE_TEXT ("FunA"));
  ACE_DEBUG ((LM_DEBUG , ACE_TEXT ("DEBUG : pid=%P Line:%l File=%N\n")));
  ACE_DEBUG ((LM_INFO  , ACE_TEXT ("INFO  : pid=%P Line:%l File=%N\n")));
  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("NOTICE: pid=%P Line:%l File=%N\n")));
  FunB();
  return;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{ int i;
  ACE_LOG_MSG->priority_mask(LM_DEBUG  | 
                             LM_INFO   |
                             LM_NOTICE |
                             LM_TRACE  , ACE_Log_Msg::PROCESS);
  ACE_TRACE(ACE_TEXT ("main"));
  for (i = 0; i < 2 ; i++ )
    FunA();
  return 0;
}
