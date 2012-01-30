#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void print_message_function (void *ptr);


int
main (void)
{
  pthread_t thread1;
  pthread_t thread2;

  printf("Starting first run\n");

  pthread_create (&thread1, NULL, (void *) &print_message_function, NULL);

  pthread_create (&thread2, NULL, (void *) &print_message_function, NULL);

  pthread_join (thread2, NULL);

  /* Not reached */
  printf("This should never happen!\n");
  pthread_join (thread1, NULL);

  return 0;
}


void
croak (void)
{
  int *ip = (int *) 17;
  char * p = sbrk(0);
    
  memset(p-1024, 42, 1024);
  write(3000, "xxx", 3);
  *ip = 7;
}


void
die (void)
{
	volatile int i= 12;
	croak ();
	i++;
	return;
}


void
print_message_function(void *dummy)
{

  printf ("Morituri te salutant!\n");
  die ();

  pthread_exit (0);
}
