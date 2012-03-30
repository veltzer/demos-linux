#include<sys/types.h> // for socket(2), recv(2), connect(2), send(2), open(2)
#include<sys/stat.h> // for open(2)
#include<sys/socket.h> // for socket(2), recv(2), connect(2), send(2), inet_addr(3)
#include<strings.h> // for bzero(3)
#include<stdio.h> // for printf(3), atoi(3), snprintf(3)
#include<stdlib.h> // for exit(3)
#include<unistd.h> // for close(2)
#include<sys/un.h> // for sockaddr_un
#include<string.h> // for strlen(3)
#include<netinet/in.h> // for sockaddr_in, inet_addr(3)
#include<arpa/inet.h> // for inet_addr(3)
#include<pthread.h> // for pthread_create(3), pthread_join(3)
#include<fcntl.h> // for open(2)

#include"us_helper.hh" // our own helper

/*
 * This is udp client demo that writes to a file anything it gets.
 *
 * 			Mark Veltzer
 */

int main(int argc,char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr,"usage: %s [host] [port] [file]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	const char* host=argv[1];
	const unsigned int port=atoi(argv[2]);
	const char* file=argv[3];

	// print 
	printf("If you want to send to this app you can just use nc like this:\n");
	printf("nc -u localhost %d < /etc/passwd\n",port);
	printf("and then:\n");
	printf("cmp /etc/passwd [yourfile]\n");

	// lets open the socket
	int sockfd;
	CHECK_NOT_M1(sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP));
	TRACE("opened socket with sockfd %d",sockfd);

	// lets create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	//server.sin_addr.s_addr=INADDR_ANY;
	server.sin_addr.s_addr=inet_addr(host);
	server.sin_port=htons(port);

	// lets bind...
	CHECK_NOT_M1(bind(sockfd,(struct sockaddr *)&server, sizeof(server)));
	TRACE("binded successfully");

	// lets create the peer address 
	struct sockaddr_in peer_addr;
	socklen_t peer_len;

	// lets recv
	unsigned int buflen=getpagesize();
	char buf[buflen];
	int ret;
	int fd;
	CHECK_NOT_M1(fd=open(file,O_WRONLY | O_CREAT | O_TRUNC,0666));
	while((ret=recvfrom(sockfd,buf,buflen,0,(struct sockaddr *)&peer_addr,&peer_len))>0) {
		TRACE("peer address is %s",inet_ntoa(peer_addr.sin_addr));
		const unsigned int prlen=20;
		char prbuf[prlen];
		int to_print=min(prlen-1,ret);
		snprintf(prbuf,to_print,"%s",buf);
		TRACE("recv %d bytes (%s)",ret,prbuf);
		int bytes=ret;
		char* pbuf=buf;
		while(bytes>0) {
			int written;
			CHECK_NOT_M1(written=write(fd,pbuf,bytes));
			bytes-=written;
			pbuf+=written;
		}
		CHECK_NOT_M1(fsync(fd));
	}
	if(ret<0) {
		perror("error in recvfrom");
		exit(EXIT_FAILURE);
	}
	CHECK_NOT_M1(close(fd));
	TRACE("closed the file");
	CHECK_NOT_M1(close(sockfd));
	TRACE("closed the socket");

	TRACE("exiting");
	return EXIT_SUCCESS;
}
