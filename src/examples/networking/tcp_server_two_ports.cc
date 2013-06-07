/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <sys/types.h>	// for socket(2), bind(2), listen(2), accept(2), recv(2), setsockopt(2)
#include <sys/socket.h>	// for socket(2), bind(2), listen(2), accept(2), recv(2), setsockopt(2), inet_addr(3)
#include <strings.h>	// for bzero(3)
#include <stdio.h>	// for printf(3), atoi(3)
#include <netdb.h>	// for getservbyname(3)
#include <arpa/inet.h>	// for ntohs(3)
#include <unistd.h>	// for read(2), close(2)
#include <pthread.h>	// for pthread_create(3)
#include <netinet/in.h>	// for sockaddr_in, inet_addr(3)
#include <arpa/inet.h>	// for inet_addr(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_ZERO_ERRNO(), CHECK_NOT_NULL(), my_max()
#include <network_utils.h>	// for get_backlog(), print_servent()
#include <sys/select.h>	// for select(2)

/*
 * This is a tcp server listening on two tcp ports using select(2)
 * and pthreads.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

// const unsigned int port=7000;
// const char* serv_name="http-alt";
// const char* serv_proto="tcp";

void *worker(void* arg) {
	int fd=*((int*)arg);
	const unsigned int buflen=1024;
	char buff[buflen];
	ssize_t res;
	do {
		res=CHECK_NOT_M1(recv(fd, buff, buflen, 0));
		ssize_t len=res;
		char* ptr=buff;
		while(len>0) {
			ssize_t curr=CHECK_NOT_M1(send(fd, ptr, len, 0));
			ptr+=curr;
			len-=curr;
		}
	} while(res>0);
	CHECK_NOT_M1(shutdown(fd, SHUT_RDWR));
	CHECK_NOT_M1(close(fd));
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	if(argc!=5) {
		fprintf(stderr, "%s: usage: %s [host] [port] [host2] [port2]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	const char* host=argv[1];
	const unsigned int port=atoi(argv[2]);
	const char* host2=argv[3];
	const unsigned int port2=atoi(argv[4]);
	printf("contact me at host %s port %d\n", host, port);
	printf("contact me at host2 %s port2 %d\n", host2, port2);

	// lets get the port number using getservbyname(3)
	// struct servent* p_servent=(struct servent*)CHECK_NOT_NULL(getservbyname(serv_name,serv_proto));
	// print_servent(p_servent);

	// lets open the socket
	int sockfd=CHECK_NOT_M1(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	printf("opened socket with sockfd %d\n", sockfd);
	int sockfd2=CHECK_NOT_M1(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	printf("opened socket with sockfd2 %d\n", sockfd2);

	// lets make the socket reusable
	int optval=1;
	CHECK_NOT_M1(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)));
	CHECK_NOT_M1(setsockopt(sockfd2, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)));
	printf("setsockopt was ok\n");

	// lets create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	// server.sin_addr.s_addr=inet_addr(host);
	server.sin_addr.s_addr=INADDR_ANY;
	// server.sin_addr.s_addr=0xff000000;
	// server.sin_port=p_servent->s_port;
	server.sin_port=htons(port);

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd, (struct sockaddr *)&server, sizeof(server)));
	server.sin_port=htons(port2);
	CHECK_NOT_M1(bind(sockfd2, (struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	// lets listen in...
	int backlog=get_backlog();
	printf("backlog is %d\n", backlog);
	CHECK_NOT_M1(listen(sockfd, backlog));
	CHECK_NOT_M1(listen(sockfd2, backlog));
	printf("listen was successful\n");
	while(true) {
		// lets select(2)
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(sockfd, &rfds);
		FD_SET(sockfd2, &rfds);
		CHECK_NOT_M1(select(my_max(sockfd, sockfd2)+1, &rfds, NULL, NULL, NULL));
		struct sockaddr_in client;
		// address length must be properly initialised
		// or the call to accept(2) will fail...
		socklen_t addrlen=sizeof(client);
		if(FD_ISSET(sockfd, &rfds)) {
			int fd=CHECK_NOT_M1(accept(sockfd, (struct sockaddr *)&client, &addrlen));
			printf("accepted fd %d\n", fd);
			// spawn a thread to handle the connection to that client...
			pthread_t thread;
			int* p=new int(fd);
			CHECK_ZERO_ERRNO(pthread_create(&thread, NULL, worker, p));
		}
		if(FD_ISSET(sockfd2, &rfds)) {
			int fd=CHECK_NOT_M1(accept(sockfd2, (struct sockaddr *)&client, &addrlen));
			printf("accepted fd %d\n", fd);
			// spawn a thread to handle the connection to that client...
			pthread_t thread;
			int* p=new int(fd);
			CHECK_ZERO_ERRNO(pthread_create(&thread, NULL, worker, p));
		}
	}
	return EXIT_SUCCESS;
}
