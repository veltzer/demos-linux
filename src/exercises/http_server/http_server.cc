/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<stdio.h> // for printf(3)
#include<sys/epoll.h> // for epoll_create(2), epoll_ctl(2), epoll_wait(2)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include<sys/types.h> // for accept4(2)
#include<sys/socket.h> // for accept4(2)
#include<netinet/in.h> // for sockaddr_in
#include<us_helper.h> // for CHECK_NOT_M1()
#include<sys/types.h> // for open(2)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for read(2), close(2)

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

int main(int argc,char** argv,char** envp) {
	if(argc!=3) {
		fprintf(stderr,"usage: %s [host] [port]\n",argv[0]);
		return EXIT_FAILURE;
	}
	const char* host=argv[1];
	const unsigned int port=atoi(argv[2]);
	printf("contact me at host %s port %d\n",host,port);

	// lets open the socket
	int sockfd;
	CHECK_NOT_M1(sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP));
	printf("opened socket with sockfd %d\n",sockfd);

	// lets make the socket reusable
	int optval=1;
	CHECK_NOT_M1(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)));
	printf("setsockopt was ok\n");

	// lets create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	//server.sin_addr.s_addr=inet_addr(host);
	server.sin_addr.s_addr=INADDR_ANY;
	//server.sin_addr.s_addr=0xff000000;
	//server.sin_port=p_servent->s_port;
	server.sin_port=htons(port);

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd,(struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	// lets listen in...
	int backlog=get_backlog();
	printf("backlog is %d\n",backlog);
	CHECK_NOT_M1(listen(sockfd,backlog));
	printf("listen was successful\n");

	// create the epoll fd
	const unsigned int max_events=10;
	int epollfd;
	CHECK_NOT_M1(epollfd=epoll_create(max_events));

	// add the listening socket to it
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;
	CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev));

	// go into the endless service loop
	while(true) {
		struct epoll_event events[max_events];
		int nfds;
		CHECK_NOT_M1(nfds=epoll_wait(epollfd, events, max_events, -1));
		for(int n=0;n<nfds;n++) {
			if(events[n].data.fd==sockfd) {
				int conn_sock;
				struct sockaddr_in local;
				socklen_t addrlen;
				CHECK_NOT_M1(conn_sock=accept4(sockfd,(struct sockaddr*)&local,&addrlen,SOCK_NONBLOCK));
				struct epoll_event ev;
				ev.events=EPOLLIN|EPOLLET;
				ev.data.fd=conn_sock;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev));
			} else {
				TRACE("got activity on fd %d",events[n].data.fd);
				const int buflen=1024;
				char buffer[buflen];
				int len;
				int fd=events[n].data.fd;
				CHECK_NOT_M1(len=read(fd,buffer,buflen));
				CHECK_NOT_M1(write(fd,buffer,len));
			}
		}
	}
	return EXIT_SUCCESS;
}
