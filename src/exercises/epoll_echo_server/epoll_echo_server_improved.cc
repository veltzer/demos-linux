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
#include <sys/timerfd.h>// for timerfd_create(2), timerfd_settime(2), timerfd_gettime(2)

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
 * to read fast (without blocking) and then you get to write fast.
 *
 * TODO:
 * - check what happens when we write large amounts of data to the output. Will the async write come up
 * short?
 */

void print_events(char* buffer, size_t size, uint32_t events) {
	char* p=buffer;
	int cursize=size;
	if(events & EPOLLIN) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLIN "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLOUT) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLOUT "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLRDHUP) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLRDHUP "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLPRI) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLPRI "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLERR) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLERR "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLHUP) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLHUP "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLET) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLET "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLONESHOT) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLONESHOT "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
}

void setup_timer(int timerfd) {
	struct itimerspec new_value;
	new_value.it_value.tv_sec=10;
	new_value.it_value.tv_nsec=0;
	new_value.it_interval.tv_sec=0;
	new_value.it_interval.tv_nsec=0;
	// dont need the old timer value
	// 0 in the flags means relative time
	CHECK_NOT_M1(timerfd_settime(timerfd, 0, &new_value, NULL));
}

int main(int argc, char** argv, char** envp) {
	if(argc!=3) {
		fprintf(stderr, "%s: usage: %s [host] [port]\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	std::map<int, int> fdmap;
	std::map<int, int> timermap;
	const char* host=argv[1];
	const unsigned int port=atoi(argv[2]);
	printf("contact me at host %s port %d\n", host, port);

	// lets open the socket
	int sockfd=CHECK_NOT_M1(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	printf("opened socket with sockfd %d\n", sockfd);

	// lets make the socket reusable
	int optval=1;
	CHECK_NOT_M1(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)));
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
	printf("bind was successful\n");

	// lets listen in...
	int backlog=get_backlog();
	printf("backlog is %d\n", backlog);
	CHECK_NOT_M1(listen(sockfd, backlog));
	printf("listen was successful\n");

	// create the epoll fd
	const unsigned int max_events=10;
	int epollfd=CHECK_NOT_M1(epoll_create(max_events));

	// add the listening socket to it
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;
	CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev));
	// go into the endless service loop
	while(true) {
		struct epoll_event events[max_events];
		int nfds=CHECK_NOT_M1(epoll_wait(epollfd, events, max_events, -1));
		for(int n=0; n<nfds; n++) {
			int currfd=events[n].data.fd;
			// is it a new connection fd?
			if(currfd==sockfd) {
				struct sockaddr_in local;
				socklen_t addrlen=sizeof(local);
				int realfd=CHECK_NOT_M1(accept4(sockfd, (struct sockaddr*)&local, &addrlen, SOCK_NONBLOCK));
				struct epoll_event ev;
				ev.events=EPOLLIN|EPOLLET|EPOLLOUT|EPOLLRDHUP;
				ev.data.fd=realfd;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, realfd, &ev));
				int timerfd=CHECK_NOT_M1(timerfd_create(CLOCK_REALTIME, 0));
				ev.events=EPOLLIN;
				ev.data.fd=timerfd;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, timerfd, &ev));
				setup_timer(timerfd);
				fdmap[realfd]=timerfd;
				timermap[timerfd]=realfd;
			}
			// is it an IO event?
			if(fdmap.find(currfd)!=fdmap.end() && events[n].events & EPOLLIN) {
				TRACE("data in");
				int realfd=currfd;
				int timerfd=fdmap.find(realfd)->second;
				const int buflen=1024;
				char buffer[buflen];
				ssize_t len=CHECK_NOT_M1(read(realfd, buffer, buflen));
				// TODO: handle short writes!
				CHECK_INT(write(realfd, buffer, len), len);
				// reset the timer on the timerfd
				setup_timer(timerfd);
			}
			// is it an disconnect event?
			if(fdmap.find(currfd)!=fdmap.end() && events[n].events & EPOLLRDHUP) {
				TRACE("disconnect event");
				int realfd=currfd;
				int timerfd=fdmap.find(realfd)->second;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, realfd, NULL));
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, timerfd, NULL));
				CHECK_NOT_M1(close(realfd));
				CHECK_NOT_M1(close(timerfd));
				fdmap.erase(realfd);
				timermap.erase(timerfd);
			}
			// is it a timer event? if so then close the connection
			if(timermap.find(currfd)!=timermap.end()) {
				TRACE("timer expired");
				int timerfd=currfd;
				int realfd=timermap.find(timerfd)->second;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, realfd, NULL));
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, timerfd, NULL));
				CHECK_NOT_M1(close(realfd));
				CHECK_NOT_M1(close(timerfd));
				fdmap.erase(realfd);
				timermap.erase(timerfd);
			}
		}
	}
	return EXIT_SUCCESS;
}
