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
#include <stdio.h>	// for printf(3)
#include <sys/epoll.h>	// for epoll_create(2), epoll_ctl(2), epoll_wait(2)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include <sys/types.h>	// for accept4(2)
#include <sys/socket.h>	// for accept4(2)
#include <netinet/in.h>	// for sockaddr_in
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_IN_RANGE()
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for read(2), close(2), write(2), getpid(3)
#include <sys/signalfd.h>	// for signalfd(2), signalfd_siginfo
#include <signal.h>	// for sigset_t, sigemptyset(3), sigaddset(3), sigprocmask(2)
#include <string.h>	// for strsignal(3)
#include <assert.h>	// for assert(3)

/*
 * This is an example of using the epoll(2) API to write an echo server using
 * just a single thread. You can test it using telnet.
 *
 * The server also listens in on signal to enable clean shutdown of the server.
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
 * TODO: check what happens when we write large amounts of data to the output. Will the async write come up short?
 */

int get_backlog() {
	// read the data from the /proc/sys/net/core/somaxconn virtual file...
	const char* filename="/proc/sys/net/core/somaxconn";
	const unsigned int size=256;
	char buf[size];
	int fd=CHECK_NOT_M1(open(filename, O_RDONLY));
	CHECK_NOT_M1(read(fd, buf, size));
	CHECK_NOT_M1(close(fd));
	return atoi(buf);
}

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

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "usage: %s [port]\n", argv[0]);
		return EXIT_FAILURE;
	}
	const unsigned int port=atoi(argv[1]);
	printf("contact me using [telnet localhost %d]\n", port);

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
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd, (struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	// lets listen in
	int backlog=get_backlog();
	printf("backlog is %d\n", backlog);
	CHECK_NOT_M1(listen(sockfd, backlog));
	printf("listen was successful\n");

	// create the epoll fd
	const unsigned int max_events=10;
	int epollfd=CHECK_NOT_M1(epoll_create(max_events));

	// add the listening socket to it
	struct epoll_event sockev;
	sockev.events=EPOLLIN;
	sockev.data.fd=sockfd;
	CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &sockev));

	// add a signal fd to the mix
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	// make sure that the signals we want to handle do not go to their
	// default handlers...
	CHECK_NOT_M1(sigprocmask(SIG_BLOCK, &mask, NULL));
	int sigfd=CHECK_NOT_M1(signalfd(-1, &mask, 0));
	printf("send SIGUSR1 to me to shut me down using [kill -s SIGUSR1 %d]\n", getpid());
	struct epoll_event sigev;
	sigev.events=EPOLLIN;
	sigev.data.fd=sigfd;
	CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, sigfd, &sigev));
	// go into the endless service loop
	while(true) {
		struct epoll_event events[max_events];
		int nfds=CHECK_NOT_M1(epoll_wait(epollfd, events, max_events, -1));
		for(int n=0; n<nfds; n++) {
			if(events[n].data.fd==sockfd) {
				struct sockaddr_in local;
				socklen_t addrlen;
				int conn_sock=CHECK_NOT_M1(accept4(sockfd, (struct sockaddr*)&local, &addrlen, SOCK_NONBLOCK));
				struct epoll_event ev;
				ev.events=EPOLLIN|EPOLLET|EPOLLOUT|EPOLLRDHUP;
				ev.data.fd=conn_sock;
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev));
				continue;
			}
			if(events[n].data.fd==sigfd) {
				// we must read the data, otherwise we will be bothered with this signal again
				// and again
				struct signalfd_siginfo fdsi;
				CHECK_INT(read(sigfd, &fdsi, sizeof(struct signalfd_siginfo)), sizeof(struct signalfd_siginfo));
				printf("got signal %d (%s)\n", fdsi.ssi_signo, strsignal(fdsi.ssi_signo));
				goto exit;
			}
			// this is regular IO handling
			TRACE("got activity on fd %d", events[n].data.fd);
			char printbuff[1024];
			print_events(printbuff, 1024, events[n].events);
			TRACE("got events %s", printbuff);
			if(events[n].events & EPOLLIN) {
				const int buflen=1024;
				char buffer[buflen];
				int fd=events[n].data.fd;
				ssize_t len=CHECK_NOT_M1(read(fd, buffer, buflen));
				// TODO: how do we know that we do not
				// block here?
				ssize_t ret=CHECK_NOT_M1(write(fd, buffer, len));
				// we really should not get blocked here
				assert(ret==len);
				TRACE("read %zd bytes and wrote %zd bytes", len, ret);
			}
			if(events[n].events & EPOLLRDHUP) {
				int fd=events[n].data.fd;
				TRACE("closing the fd %d", fd);
				CHECK_NOT_M1(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL));
				CHECK_NOT_M1(close(fd));
			}
		}
	}
exit:
	// TODO:
	// - close the socket
	// - close the signal fd
	// - clean the epollfd
	return EXIT_SUCCESS;
}
