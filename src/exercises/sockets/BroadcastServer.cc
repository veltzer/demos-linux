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

#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	int brsock, sendsock;
	ssize_t datalen;
	socklen_t fromaddrlen;
	struct sockaddr_in server, fromaddr;
	time_t t;
	char ibuffer[1000], obuffer[1000];
	if((brsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("brsock socket failed");
		exit(errno);
	}
	/*
	int on = 1;
	if(setsockopt(brsock, SOL_SOCKET, SO_BROADCAST, &on, sizeof on) == -1) {
		perror("setsockoption failed");
		exit(errno);
	}
	*/
	bzero(& server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6969); // should use getservbyname()

	if(bind(brsock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("brsock connect failed");
		exit(errno);
	}
	while(true) {
		if((datalen = recvfrom(brsock, ibuffer, sizeof(ibuffer), 0, (struct sockaddr *) & fromaddr, & fromaddrlen)) == -1) {
			perror("brsock recvfrom failed");
			exit(errno);
		}
		ibuffer[datalen-1] = '\0'; // get rid of '\n'
		printf("Got ==>%s<==\n", ibuffer);
		sprintf(obuffer, "Bad request");
		if(strcmp(ibuffer, "date") == 0) {
			t = time(NULL);
			sprintf(obuffer, "%s", ctime(& t));
		}
		if(strcmp(ibuffer, "pid") == 0) {
			sprintf(obuffer, "%d", getpid());
		}
		if((sendsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("sendsock socket failed");
			exit(errno);
		}
		fromaddr.sin_port = htons(6996); // reply port id
		if(connect (sendsock, (struct sockaddr *) & fromaddr, fromaddrlen) == -1) {
			perror("sendsock connect failed");
			exit(errno);
		}
		ssize_t ret=write(sendsock, obuffer, strlen(obuffer));
		if(ret!=(ssize_t)strlen(obuffer)) {
			perror("write(2) error");
		}
		close(sendsock);
	}
	return EXIT_SUCCESS;
}
