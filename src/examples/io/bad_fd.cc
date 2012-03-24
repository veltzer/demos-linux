#include <stdio.h> // for perror(3)
#include <unistd.h> // for read(2), getdtablesize(2)

/*
 * This is an example of what happens when you read from a bad fd.
 * The important thing to note is that you do not SEGFAULT. Technically
 * you called a syscall with the wrong parameter and what you get back
 * is an error.
 * Note yet that read(2) return type is ssize_t which is SIGNED as opposed to size_t
 * which is UNSIGNED. The idea here is the ability to return -1 and report errors
 * this way.
 *
 * Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	const size_t SIZE = 5;
	const int BAD_FD = getdtablesize()+1;
	char buf[SIZE];
	ssize_t s = read(BAD_FD, buf, SIZE);

	if (s == -1) {
		perror("read");
		return(-1);
	}
	return(0);
}
