#include <sys/types.h> // for socket(2), bind(2)
#include <sys/socket.h> // for socket(2), bind(2)
#include <strings.h> // for bzero(3)
#include <stdio.h> // for perror(3), printf(3)
#include <errno.h> // for errno
#include <netdb.h> // for getservbyname(3)
#include <arpa/inet.h> // for ntohs(3)

#include "us_helper.hh" // our own helper

/*
 * This is a demo of a simple echo socket server implementation in pure C in Linux
 *
 * 			Mark Veltzer
 */

//const unsigned int port=7000;
const char* serv_name="http-alt";
const char* serv_proto="tcp";

void print_servent(struct servent* p_servent) {
	printf("name is %s\n",p_servent->s_name);
	printf("proto is %s\n",p_servent->s_proto);
	printf("port is %d (network order its %d)\n",ntohs(p_servent->s_port),p_servent->s_port);
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
	int sock;
	CHECK_NOT_M1(sock=socket(AF_INET,SOCK_DGRAM,0));
	printf("opened socket with fd %d\n",sock);

	// lets create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=p_servent->s_port;

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sock,(struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	/*
	while(true) {
		if ((datalen = recvfrom(brsock, ibuffer, sizeof(ibuffer), 0, (struct sockaddr *)&fromaddr, &fromaddrlen)) == -1) {
			perror("brsock recvfrom failed");
			exit(errno);
		}
		ibuffer[datalen - 1] = '\0'; // get rid of '\n'
		printf("Got ==>%s<==\n", ibuffer);
		sprintf(obuffer, "Bad request");
		if (strcmp(ibuffer, "date") == 0) {
			t = time(NULL);
			sprintf(obuffer, "%s", ctime(&t));
		}
		if (strcmp(ibuffer, "pid") == 0) {
			sprintf(obuffer, "%d", getpid());
		}

		if ((sendsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("sendsock socket failed");
			exit(errno);
		}
		// reply port id
		fromaddr.sin_port = htons(6996);
		if (connect(sendsock, (struct sockaddr *)&fromaddr, fromaddrlen) == -1) {
			perror("sendsock connect failed");
			exit(errno);
		}
		size_t size = write(sendsock, obuffer, strlen(obuffer));
		if (size != strlen(obuffer)) {
			perror("sendsock connect failed");
			exit(errno);
		}
		close(sendsock);
	}
	*/
	return 0;
}
