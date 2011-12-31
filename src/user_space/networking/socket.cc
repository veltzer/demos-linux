#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
 * This is a demo of a socket server implementation in pure C in Linux
 *
 * 			Mark Veltzer
 */

int main(int argc,char** argv, char** envp) {
	int brsock, sendsock;
	ssize_t datalen;
	socklen_t fromaddrlen;
	struct sockaddr_in server, fromaddr;
	time_t t;
	char ibuffer[1000], obuffer[1000];

	if ((brsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("brsock socket failed");
		exit(errno);
	}

	/*    int on = 1;
	 *  if (setsockopt(brsock, SOL_SOCKET, SO_BROADCAST,
	 *                             &on, sizeof on) == -1){
	 *      perror("setsockoption failed");
	 *      exit(errno);
	 *  }
	 */
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6969);                                                                                                            // should use getservbyname()

	if (bind(brsock, (struct sockaddr *)&server,
	         sizeof(server)) < 0) {
		perror("brsock connect failed");
		exit(errno);
	}

	while (1) {
		if ((datalen = recvfrom(brsock, ibuffer, sizeof(ibuffer), 0,
		                        (struct sockaddr *)&fromaddr,
		                        &fromaddrlen)) == -1) {
			perror("brsock recvfrom failed");
			exit(errno);
		}
		ibuffer[datalen - 1] = '\0';                                                                                                                                                                                                          // get rid of '\n'
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
		if (connect(sendsock, (struct sockaddr *)&fromaddr,
		            fromaddrlen) == -1) {
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
	exit(0);
}
