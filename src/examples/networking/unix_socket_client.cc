/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2023 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <sys/types.h>	// for socket(2), recv(2), connect(2), send(2)
#include <sys/socket.h>	// for socket(2), recv(2), connect(2), send(2)
#include <strings.h>	// for bzero(3)
#include <stdio.h>	// for printf(3), atoi(3), snprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>	// for close(2)
#include <sys/un.h>	// for sockaddr_un
#include <string.h>	// for strlen(3)
#include <err_utils.h>	// for CHECK_NOT_M1()

/*
 * This is a unix socket client demo.
 */

const char* filename="/tmp/myunixsocket";

int main(int argc, char** argv, char** envp) {
	if(argc!=1) {
		fprintf(stderr, "%s: usage: %s\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// lets open the socket
	int sockfd=CHECK_NOT_M1(socket(AF_UNIX, SOCK_STREAM, 0));
	printf("opened socket with sockfd %d\n", sockfd);

	// lets create the address
	struct sockaddr_un server;
	bzero(&server, sizeof(server));
	server.sun_family=AF_UNIX;
	snprintf(server.sun_path, 108, "%s", filename);

	// lets connect...
	CHECK_NOT_M1(connect(sockfd, (struct sockaddr *)&server, sizeof(server)));
	printf("connected successfully\n");

	// lets send "hello"...
	const char* sbuf="hello";
	unsigned int sbuflen=strlen(sbuf);
	printf("sending [%s]\n", sbuf);
	CHECK_NOT_M1(send(sockfd, sbuf, sbuflen, 0));

	// lets receive
	unsigned int rbuflen=1024;
	char* rbuf=new char[rbuflen];
	int ret=CHECK_NOT_M1(recv(sockfd, rbuf, rbuflen, 0));
	char* prbuf=new char[rbuflen];
	snprintf(prbuf, ret+1, "%s", rbuf);
	printf("received [%s]\n", prbuf);
	delete[] rbuf;
	delete[] prbuf;

	// lets close...
	CHECK_NOT_M1(close(sockfd));
	return EXIT_SUCCESS;
}
