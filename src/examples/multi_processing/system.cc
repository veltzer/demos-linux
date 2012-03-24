#include <stdlib.h> // for system(3)
#include <stdio.h> // for printf(3)

/*
 * This examples demostrates how to parse the return code
 * from system(3)
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int res = system("/bin/ls /foo");

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
