#include <stdlib.h> // for atoi(3)
#include <stdio.h> // for printf(3)
#include <sys/types.h> // for WIF(3)
#include <sys/wait.h> // for WIF(3)

/*
 * This executable receives the status code (exit code) of some process and prints
 * an analysis of what the code means. Useful in case where you are not sure what
 * the status code means or as a demo of how to analyze such a code. 
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	if(argc!=2) {
		printf("usage: prog [status code]\n");
		return -1;
	}
	int res = atoi(argv[1]);
	printf("analyzing code %d\n",res);

	if (WIFSIGNALED(res)) {
		printf("ahm... Child was killed by os with signal %d\n", WTERMSIG(res));
	}
	if (WIFEXITED(res)) {
		printf("Child was NOT killed by OS.\n");
		int return_code = WEXITSTATUS(res);
		if (return_code) {
			printf("Child exited successfully but reported error %d\n", return_code);
		} else {
			printf("Child was a success\n");
		}
	}
	return(0);
}
