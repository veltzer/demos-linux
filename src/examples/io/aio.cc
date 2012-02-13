#include <stdio.h> // for printf(3)
#include <aio.h> // for aio_*(3)
#include <strings.h> // for bzero(3)
#include <stdlib.h> // for malloc(3)

#include "us_helper.hh" // our own helper

/*
 * This is an example of asynchroneous IO.
 *
 * References:
 * man aio.h
 * http://www.ibm.com/developerworks/linux/library/l-async/
 *
 *              Mark Veltzer
 * EXTRA_LIBS=-lrt
 */
int main(int argc, char **argv, char **envp) {
	const int BUFSIZE=1024;
	int fd;
	CHECK_NOT_M1(fd=open("/etc/passwd",O_RDONLY));
	/* Allocate a data buffer for the aiocb request */
	void* ptr;
	CHECK_NOT_NULL(ptr=malloc(BUFSIZE+1));
	struct aiocb my_aiocb;
	/* Zero out the aiocb structure (recommended) */
	bzero(&my_aiocb,sizeof(struct aiocb));
	/* Initialize the necessary fields in the aiocb */
	my_aiocb.aio_buf=ptr;
	my_aiocb.aio_fildes=fd;
	my_aiocb.aio_nbytes=BUFSIZE;
	my_aiocb.aio_offset=0;
	CHECK_ZERO(aio_read(&my_aiocb));
	// busy wait (bad!)
	//while(aio_error(&my_aiocb)==EINPROGRESS);
	// sleeping wait via API
	const struct aiocb * const cblist[]={ &my_aiocb };
	CHECK_ZERO(aio_suspend(cblist,1,NULL));
	int ret;
	CHECK_NOT_M1(ret=aio_return(&my_aiocb));
	printf("got ret of %d\n",ret);
	char* buf=(char*)my_aiocb.aio_buf;
	buf[ret]='\0';
	printf("%s\n",buf);
	return(0);
}
