#include <stdio.h> // for printf(3), fprintf(3)
#include <strings.h> // for bzero(3)
#include <stdlib.h> // for malloc(3), atoi(3)
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)

#include "us_helper.hh" // our own helper
#include "Stat.h" // our own helper

/*
 * This example explores the performance of the write system call...
 *
 * The example is supposed to show that writes to /dev/null have
 * constant performance while writes to a file are different: when
 * the OS buffers are empty they are fast (memcpy to kernel) and when
 * they are full they block...
 *
 *	Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	if(argc!=4) {
		fprintf(stderr,"usage: %s [filename] [bufsize] [count]\n",argv[0]);
		return -1;
	}
	const char* filename=argv[1];
	const unsigned int bufsize=atoi(argv[2]);
	const unsigned int count=atoi(argv[3]);
	void* buf=malloc(bufsize);
	int fd;
	Stat s(
		500, // bin number
		1000,
		1000
	);
	CHECK_NOT_M1(fd=open(filename,O_RDWR | O_CREAT,0666));
	for(unsigned int i=0;i<count;i++) {
		ticks_t t1=getticks();
		CHECK_NOT_M1(write(fd,buf,bufsize));
		ticks_t t2=getticks();
		s.accept(t2-t1);
	}
	CHECK_NOT_M1(close(fd));
	s.print();
	return EXIT_SUCCESS;
}
