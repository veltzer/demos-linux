/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// This example is from the ACE Programmers Guide.
////  Chapter:  "Thread Management"
//// For details please see the guide at
//// http://www.cs.wustl.edu/~schmidt/ACE.html
////  AUTHOR: Umar Syyid (usyyid@hns.com)
//// and Ambreen Ilyas (ambreen@bitsmart.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Example 1
#include<ace/Thread.h>
#include<ace/OS.h>
#include<ace/Synch_T.h>
#include<ace/Synch.h>
static int number=0;
static int seed=0;

static void* worker(void *arg)
   { ACE_UNUSED_ARG(arg);
     ACE_DEBUG((LM_DEBUG,"Thread (%t) Created to do some work"));
     ::number++;
     ACE_DEBUG((LM_DEBUG," and number is %d\n",::number));
 
     //Let the other guy go while I fall asleep for a random period of time
     ACE_Thread::yield();
     ACE_OS::sleep(ACE_OS::rand()%2);

     //Exiting now
     ACE_DEBUG((LM_DEBUG, "\t\t Thread (%t) Done! \t The number is now: %d\n",number));
     // --- ACE_OS::fflush(stdout);
     return 0;
   }
 

int main(int argc, char *argv[]){
if ( argc < 2 )
    ACE_DEBUG((LM_DEBUG,"Usage: <program_name> <number of threads>\n"));
 
int n_threads=ACE_OS::atoi(argv[1]);
//Setup the random number generator
ACE_OS::srand(::seed);

//Spawn off n_threads number of threads
for(int i=0; i < n_threads; i++){
 if( ACE_Thread::spawn((ACE_THR_FUNC)worker) ==-1)
  ACE_DEBUG((LM_DEBUG,"Error in spawning thread\n"));
 }

// Wait for all the threads to exit before you let the main fall through
// and have the process exit. This way of using join is non-portable
// and may not work on a system using pthreads.
 int check_count=0;
//  sleep(30);
 while ( ACE_Thread::join(NULL, NULL, NULL ) == 0)
         check_count++;
 ACE_DEBUG((LM_DEBUG,"Eytan Was here\n"));
 ACE_ASSERT(check_count==n_threads);
}
