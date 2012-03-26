#include <sys/types.h> // for socket(2), recv(2), connect(2), send(2)
#include <sys/socket.h> // for socket(2), recv(2), connect(2), send(2)
#include <strings.h> // for bzero(3)
#include <stdio.h> // for printf(3), atoi(3), snprintf(3)
#include <stdlib.h> // for exit(3)
#include <unistd.h> // for close(2)
#include <sys/un.h> // for sockaddr_un
#include <string.h> // for strlen(3)

#include "us_helper.hh" // our own helper

/*
 * This is a unix socket client demo.
 *
 * 			Mark Veltzer
 */

const char* filename="/tmp/myunixsocket";

int main(int argc,char** argv, char** envp) {
	if(argc!=1) {
		fprintf(stderr,"usage: %s\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// lets open the socket
	int sockfd;
	CHECK_NOT_M1(sockfd=socket(AF_UNIX,SOCK_STREAM,0));
	printf("opened socket with sockfd %d\n",sockfd);

	// lets create the address
	struct sockaddr_un server;
	bzero(&server, sizeof(server));
	server.sun_family=AF_UNIX;
	snprintf(server.sun_path,108,"%s",filename);

	// lets connect...
	CHECK_NOT_M1(connect(sockfd,(struct sockaddr *)&server, sizeof(server)));

	// lets send "hello"...
	const char* sbuf="hello";
	unsigned int sbuflen=strlen(sbuf);
	printf("sending [%s]\n",sbuf);
	CHECK_NOT_M1(send(sockfd,sbuf,sbuflen,0));

	// lets receive
	unsigned int rbuflen=1024;
	char rbuf[rbuflen];
	int ret;
	CHECK_NOT_M1(ret=recv(sockfd,rbuf,rbuflen,0));
	char prbuf[rbuflen];
	snprintf(prbuf,ret+1,"%s",rbuf);
	printf("received [%s]\n",prbuf);

	// lets close...
	CHECK_NOT_M1(close(sockfd));
	return EXIT_SUCCESS;
}
