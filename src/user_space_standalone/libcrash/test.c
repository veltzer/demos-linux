#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "crash.h"

void print_message_function (void *ptr);
char buf[128];

int
main ( int argc, char * argv[])
{
  pthread_t thread1;
  pthread_t thread2;
  int ret = 0;
    
  /* Prepare asser buffer */
  strcat(buf, (char *)"We're doomed!");

  printf("Starting first run\n");
  fflush(NULL);

  /* register */
  ret = register_crash_handler(argv[0],(unsigned char *)&buf);
  assert(ret==0);

  /* create two threads and let them race */
  pthread_create (&thread1, NULL, (void *) &print_message_function, NULL);
  
  pthread_create (&thread2, NULL, (void *) &print_message_function, NULL);

  pthread_join (thread2, NULL);

  /* Not reached */
  printf("This should never happen!\n");
  fflush(NULL);
  assert(0);
  pthread_join (thread1, NULL);

  return 0;
}


void * kill_malloc(size_t size, const void *caller) {
	printf("Malloc called from %p\n", caller);
	abort();
}

void * kill_realloc(void *ptr, size_t size, const void *caller) {
	printf("Realloc called from %p\n", caller);
	abort();
}

void kill_free(void *ptr, const void *caller) {
	printf("Free called from %p\n", caller);
	abort();
}

void * kill_memalign(size_t alignment, size_t size, const void *caller) {
	printf("Memalign called from %p\n", caller);
	abort();
}

/* This function generaters a fault.
 * We try to REALLY be nasty and screw things up bad.
 */

void
croak (void)
{
  int *ip = (int *) 17;
  char * p = sbrk(0);
  
  /* try to catch implicit malloc calls */
   __malloc_hook = kill_malloc;
   __realloc_hook = kill_realloc;
   __free_hook = kill_free;
   __memalign_hook = kill_memalign;
    
  
  /* Corrupt the malloc arena as a real fault would do. 
   */ 
  memset(p-1024, 42, 1024);
  
  /* Do a simple system that fails so that errno has some interesting
   * value to check 
   */ 
  write(3000, "xxx", 3);
  
  /* Try to put 7 in address 17. This is an illegal memory access.
   * Sit back and watch the fire works...
   */
  *ip = 7;
}



/* A filler function so that we'll have a meanigful stack.
 * The volatile int is used to keep the compiler from optimizing
 * this function away
 */

void
die (void)
{
  volatile int i= 12;
  croak ();
  i++;
  return;
}


/* The test thread function */
void
print_message_function(void *dummy)
{

  /* Latin: "those who about to die sallute you". */
  printf ("Morituri te salutant!\n");
  fflush(NULL);
  
  /* Call the crasher functions */
  die ();

  pthread_exit (0);
}

/* THE END */
