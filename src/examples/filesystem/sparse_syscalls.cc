#include<stdio.h> // for perror(3)
#include<sys/types.h> // for open(2), lseek(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for write(2), lseek(2), close(2)
#include<stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE

/*
 * This example explores sparse files on a UNIX system.
 *
 * 			Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	const char *buf = "hello";
	int fd = open("sparse_file", O_CREAT | O_RDWR, 0666);
	int res = fd;

	if (res == -1) {
		perror("error in open");
		exit(EXIT_FAILURE);
	}
	res = lseek(fd, 1000000, SEEK_CUR);
	if (res == -1) {
		perror("error in seek");
		exit(EXIT_FAILURE);
	}
	res = write(fd, buf, 5);
	if (res == -1) {
		perror("error in write");
		exit(EXIT_FAILURE);
	}
	res = close(fd);
	if (res == -1) {
		perror("error in close");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
