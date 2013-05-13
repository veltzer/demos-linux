#include <stdio.h> // for fprintf(3), fopen(3)
#include <stdlib.h> // for malloc(3), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h> // for pause(2), getpid(2), sleep(3), read(2)
#include <sys/types.h> // for getpid(2), open(2)
#include <sys/mman.h> // for mprotect(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <us_helper.h> // for CHECK_NOT_M1(), CHECK_INT()

/*
 * This example shows that you can continue to use a file after it has
 * been removed from the hard disk. So: as long as a process is using
 * a file its content will not be deleted from the hard drive.
 *
 * You must create the /tmp/myfile.data file, make sure it has
 * 3 bytes and a number int. Remove it as this process runs to
 * see it in action.
 */

int main(int argc,char** argv,char** envp) {
	int fd=CHECK_NOT_M1(open("/tmp/myfile.data",O_RDONLY));
	const unsigned int bufsize=1024;
	const unsigned int readsize=3;
	char buffer[bufsize];
	unsigned int counter=0;
	while(true) {
		CHECK_INT(read(fd,buffer,bufsize),readsize);
		int a;
		CHECK_INT(sscanf(buffer,"%d",&a),1);
		fprintf(stderr,"number is %d, counter is %d\n",a,counter);
		counter++;
		sleep(1);
		CHECK_NOT_M1(lseek(fd,0,SEEK_SET));
	}
	CHECK_NOT_M1(close(fd));
	return EXIT_SUCCESS;
}
