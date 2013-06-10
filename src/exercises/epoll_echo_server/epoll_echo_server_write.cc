/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <sys/epoll.h>	// for epoll_create(2), epoll_ctl(2), epoll_wait(2)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include <sys/types.h>	// for accept4(2)
#include <sys/socket.h>	// for accept4(2)
#include <netinet/in.h>	// for sockaddr_in
#include <unistd.h>	// for read(2), close(2), write(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_IN_RANGE(), CHECK_INT()
#include <network_utils.h>	// for get_backlog()
#include <map>	// for std::map<T1,T2>, std::map<T1,T2>::iterator

/*
 * This is a solution to the echo server exercise.
 *
 * NOTES:
 * - events arrive together. One epoll_wait can wake you up on multiple events on the same fd.
 * - EPOLLRDHUP is the event delivered when the other side hangs up.
 * - EPOLLOUT is the event delivered when writing is done. If we work edge triggered then
 * it will be delivered only once. We are supposed to check that the entire write is done.
 * - to stop polling on an fd you must first deregister it from epoll AND ONLY THEN close it (obvious).
 * - we are doing async IO here all over. This means that when you are notified that there is data you
 * want to read fast (without blocking) and then you want to write fast.
 * - this solution relies on the fact that we can write writhout blocking. If we come up short then
 * we crash. This is not really a good idea. See an improvement to this exercise for a better solution.
 */

typedef struct bufdata {
	char* buffer;
	unsigned int size;
	unsigned int write_pos;
	unsigned int read_pos;
} bufdata;

int main(int argc, char** argv, char** envp) {
	if(argc!=4) {
		fprintf(stderr, "%s: usage: %s [host] [port] [bufsize]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// get the parameters
	const char* host=argv[1];
	const unsigned int port=atoi(argv[2]);
	const unsigned int bufsize=atoi(argv[3]);

	// open the socket
	int sockfd=CHECK_NOT_M1(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	// make the socket reusable
	int optval=1;
	CHECK_NOT_M1(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)));

	// create the address
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);

	// bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd, (struct sockaddr *)&server, sizeof(server)));

	// listen
	int backlog=get_backlog();
	CHECK_NOT_M1(listen(sockfd, backlog));

	// create the epollfd
	const unsigned int max_events=10;
	int epollfd=CHECK_NOT_M1(epoll_create(max_events));

	// add the listening socket to it
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;
	CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev));

	// data structures
	std::map<int, bufdata> fdbuffermap;

	// message to the user
	printf("contact me at host %s port %d\n", host, port);

	// go into the endless service loop
	while(true) {
		struct epoll_event events[max_events];
		int nfds=CHECK_NOT_M1(epoll_wait(epollfd, events, max_events, -1));
		for(int n=0; n<nfds; n++) {
			int currfd=events[n].data.fd;
			// someone is trying to connect
			if(currfd==sockfd) {
				struct sockaddr_in local;
				socklen_t addrlen=sizeof(local);
				int realfd=CHECK_NOT_M1(accept4(sockfd, (struct sockaddr*)&local, &addrlen, SOCK_NONBLOCK));
				struct epoll_event ev;
				ev.events=EPOLLIN|EPOLLRDHUP;
				ev.data.fd=realfd;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, realfd, &ev));
				// put the data in the map
				bufdata bd;
				bd.buffer=(char*)CHECK_NOT_NULL(malloc(bufsize*sizeof(char)));
				bd.size=0;
				bd.read_pos=0;
				bd.write_pos=0;
				fdbuffermap[realfd]=bd;
			}
			if(fdbuffermap.find(currfd)!=fdbuffermap.end() && events[n].events & EPOLLIN) {
				int realfd=currfd;
				char* buffer=fdbuffermap.find(realfd)->second.buffer;
				int read_pos=fdbuffermap.find(realfd)->second.read_pos;
				ssize_t len=CHECK_NOT_M1(read(realfd, buffer+read_pos, bufsize));
				fdbuffermap.find(realfd)->second.read_pos+=len;
				// now mark this fd as waiting for write
				ev.events=EPOLLIN|EPOLLRDHUP|EPOLLOUT;
				ev.data.fd=realfd;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_MOD, realfd, &ev));
			}
			if(fdbuffermap.find(currfd)!=fdbuffermap.end() && events[n].events & EPOLLOUT) {
				int realfd=currfd;
				char* buffer=fdbuffermap.find(realfd)->second.buffer;
				int read_pos=fdbuffermap.find(realfd)->second.read_pos;
				int write_pos=fdbuffermap.find(realfd)->second.write_pos;
				CHECK_INT(write(realfd, buffer+write_pos, read_pos-write_pos), read_pos-write_pos);
			}
			if(fdbuffermap.find(currfd)!=fdbuffermap.end() && events[n].events & EPOLLRDHUP) {
				int realfd=currfd;
				char* buffer=fdbuffermap.find(realfd)->second.buffer;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, realfd, NULL));
				CHECK_NOT_M1(close(realfd));
				free(buffer);
			}
		}
	}
	return EXIT_SUCCESS;
}
