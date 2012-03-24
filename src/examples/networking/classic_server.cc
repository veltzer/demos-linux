#include <sys/types.h> // for socket(2), bind(2), open(2), listen(2), accept(2), recv(2)
#include <sys/socket.h> // for socket(2), bind(2), listen(2), accept(2), recv(2)
#include <strings.h> // for bzero(3)
#include <stdio.h> // for perror(3), printf(3), atoi(3)
#include <errno.h> // for errno
#include <netdb.h> // for getservbyname(3)
#include <arpa/inet.h> // for ntohs(3)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for read(2), close(2)
#include <pthread.h> // for pthread_create(3)

#include "us_helper.hh" // our own helper

/*
 * This is a demo of a simple echo socket server implementation in pure C in Linux
 *
 * 			Mark Veltzer
 * EXTRA_LIBS=-lpthread
 */

//const unsigned int port=7000;
const char* serv_name="http-alt";
const char* serv_proto="tcp";

int get_backlog() {
	// read the data from the /proc/sys/net/core/somaxconn virtual file...
	const char* filename = "/proc/sys/net/core/somaxconn";
	const unsigned int size = 256;
	char buf[size];
	int fd;
	CHECK_NOT_M1(fd = open(filename, O_RDONLY));
	CHECK_NOT_M1(read(fd, buf, size));
	CHECK_NOT_M1(close(fd));
	return atoi(buf);
}

void print_servent(struct servent* p_servent) {
	printf("name is %s\n",p_servent->s_name);
	printf("proto is %s\n",p_servent->s_proto);
	printf("port is %d (network order its %d)\n",ntohs(p_servent->s_port),p_servent->s_port);
}

void *worker(void* arg) {
	int fd=(int)arg;
	TRACE("thread %d starting",gettid());
	TRACE("thread %d got fd %d",gettid(),fd);
	const unsigned int buflen=1024;
	char buff[buflen];
	ssize_t res;
	CHECK_NOT_M1(res=recv(fd,buff,buflen,0));
	while(res!=0) {
		CHECK_NOT_M1(send(fd,buff,res,0));
		CHECK_NOT_M1(res=recv(fd,buff,buflen,0));
	}
	CHECK_NOT_M1(close(fd));
	TRACE("thread %d ending",gettid());
	return NULL;
}

int main(int argc,char** argv, char** envp) {
	//ssize_t datalen;
	//socklen_t fromaddrlen;
	//time_t t;
	//char ibuffer[1000], obuffer[1000];

	// lets get the port number using getservbyname(3)
	struct servent* p_servent;
	CHECK_NOT_NULL(p_servent=getservbyname(serv_name,serv_proto));
	print_servent(p_servent);

	// lets open the socket
	int sockfd;
	CHECK_NOT_M1(sockfd=socket(AF_INET,SOCK_STREAM,0));
	printf("opened socket with sockfd %d\n",sockfd);

	// lets create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=p_servent->s_port;

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd,(struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	// lets listen in...
	int backlog=get_backlog();
	printf("backlog is %d\n",backlog);
	CHECK_NOT_M1(listen(sockfd,backlog));
	printf("listen was successful\n");

	while(true) {
		int fd;
		struct sockaddr_in client;
		socklen_t addrlen;
		CHECK_NOT_M1(fd=accept(sockfd,(struct sockaddr *)&client,&addrlen));
		printf("accepted fd %d\n",fd);
		// spawn a thread to handle the connection to that client...
		pthread_t thread;
		CHECK_ZERO(pthread_create(&thread, NULL, worker, (void*)fd));
	}
	return 0;
}
