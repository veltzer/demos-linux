#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>

sem_t my_sem;
void print_message_function (void *ptr);

#define pthread_attr_default NULL


/* Obtain a backtrace and print it to stdout. */
void
print_trace (void)
{
  void *array[25];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 25);
  strings = backtrace_symbols (array, size);
  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
    printf ("%s\n", strings[i]);

  free (strings);
}

/* The fault handler function */
void
fault_handler (int signal, siginfo_t * siginfo, void *context)
{

  /* Using printf, and malloc/free is not safe from inside an 
   * exception singal handler. But for the example it's OK. 
   * Just don't use this in real code...
   */
  printf ("\n*** WOOOOHA! Caught a signal... ***\n");
  if (siginfo)
    {
      printf ("Handling signal %d from address %p\n", signal,
	      siginfo->si_addr);
    }

  print_trace ();
  abort();
}


int
main (void)
{
  pthread_t thread1, thread2;
  char *message1 = "Hello";
  char *message2 = "World";
  char buf[255];

  sem_init (&my_sem, 0, 0);

  pthread_create (&thread1, pthread_attr_default,
		  (void *) &print_message_function, (void *) message1);

  sem_wait (&my_sem);

  pthread_create (&thread2, pthread_attr_default,
		  (void *) &print_message_function, (void *) message2);


  pthread_join (thread1, NULL);
  pthread_join (thread2, NULL);

  printf ("!\n");
  sem_destroy (&my_sem);
  exit (0);
}


void
croak (void)
{

  int *ip = (int *) 17;

  *ip = 7;

}


void
die (void)
{

  croak ();
}

do_signal_thingie (void)
{

  struct sigaction act;

  memset (&act, 0, sizeof (act));
  act.sa_sigaction = fault_handler;
  sigemptyset (&act.sa_mask);
  act.sa_flags = SA_SIGINFO;
  sigaction (SIGSEGV, &act, NULL);

}

void
print_message_function (void *ptr)
{
  char *message;
  message = (char *) ptr;

  do_signal_thingie ();
  printf ("%s ", message);

  sem_post (&my_sem);

  die ();
  
  pthread_exit (0);
}
