#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

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
		exit(1);
	}
	res = lseek(fd, 1000000, SEEK_CUR);
	if (res == -1) {
		perror("error in seek");
		exit(1);
	}
	res = write(fd, buf, 5);
	if (res == -1) {
		perror("error in write");
		exit(1);
	}
	res = close(fd);
	if (res == -1) {
		perror("error in close");
		exit(1);
	}
	return EXIT_SUCCESS;
}
