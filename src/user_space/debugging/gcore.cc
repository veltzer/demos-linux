#include <stdio.h> // for printf(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2), sleep(3), fork(2)

#include "us_helper.hh"

#define DO_SLEEP
//#define DO_SELF_PHOTO
#define DO_FORK

/*
 *      Explore usage of command line gcore.
 *
 *      This simple process simply increases a counter and prints it in a slow
 *      manner.
 *
 *      Take a photo of this process in action using gcore and then debug it
 *      and see if you get the right value...
 *
 *      I had to make i volatile since the compiler would put it in a register
 *      and optimize it out.
 *
 *      Why do I do the fork at the begining? It seems that newer linux distributions
 *      do not allow ptrace (which is at the core of gcore) of any process to any process
 *      (ubuntu is a prime example). The default policy (which can be configured by /proc)
 *      is that only the parent of a process can photo the child.
 *
 *      Actually, I'm using sudo here so I'm not really using ptrace right. This is because
 *      the process that I'm taking a photo of and the gcore process are siblings and not
 *      parent-child. There is probably a way to make this work if we link with libgdb and call
 *      gcore functionality directly from our code.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
#ifdef DO_FORK
	pid_t child_pid=fork();
	if(child_pid==0) {
#endif // DO_FORK
		printf("take a photo of me with 'gcore %d'...\n",getpid());
		for (volatile unsigned int i = 0; i < 10000; i++) {
			printf("i is %d\n", i);
#ifdef DO_SLEEP
			usleep(5);
#endif // DO_SLEEP
#ifdef DO_SELF_PHOTO
			if(i==5000) {
				my_system("sudo gcore %d",getpid());
			}
#endif // DO_SELF_PHOTO
		}
		return(0);
#ifdef DO_FORK
	} else {
		my_system("sudo gcore %d",child_pid);
	}
#endif // DO_FORK
}
