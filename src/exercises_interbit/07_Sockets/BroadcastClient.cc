#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>

int main(int argc,char** argv,char** envp) {
	int on, got, brsock, rplysock, newsock;
	ssize_t datalen;
	socklen_t rplyFromAddressLen;
	struct sockaddr_in braddress, rplyFromAddress, myAddress;
	char ibuffer[1000], obuffer[1000];
	int nfds;
	struct timeval timeout;
	fd_set readfds;

	if ((brsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("brsock socket failed");
		exit(errno);
	}

	bzero(& braddress, sizeof(braddress));
	braddress.sin_family = AF_INET;
	braddress.sin_addr.s_addr = INADDR_BROADCAST;
	braddress.sin_port = htons(6969); // should use getservbyname()

	on = 1;
	if (setsockopt(brsock, SOL_SOCKET, SO_BROADCAST, &on, sizeof on) < 0)
	{
		perror("brsock setsockopt failed");
		exit(errno);
	}
	if ((rplysock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("rplysock socket failed");
		exit(errno);
	}
	bzero(& myAddress, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_port = htons(6996); // reply port id
	myAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind (rplysock, (struct sockaddr *) & myAddress, sizeof(myAddress)) == -1)
	{
		perror("rplysock bind failed");
		exit(errno);
	}
	if (listen(rplysock, SOMAXCONN) == -1) // 128 in Linux 5 in BSD
	{
		perror("listen failed");
		exit(errno);
	}
	printf("Type your request: ");
	while (fgets(obuffer, sizeof(obuffer), stdin))
	{
		if ((datalen = sendto(brsock, obuffer, strlen(obuffer), 0,
			(struct sockaddr *) & braddress, sizeof(braddress))) == -1)
		{
			perror("brsock sendto failed");
			exit(errno);
		}
		printf("Request sent\n");
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		FD_ZERO(& readfds);
		FD_SET(rplysock, & readfds);
		while ((nfds = select(rplysock+1, & readfds, NULL, NULL, & timeout)) > 0)
		{
			newsock = accept(rplysock, (struct sockaddr *) & rplyFromAddress,
				& rplyFromAddressLen);
			while ((got = read(newsock, ibuffer, sizeof(ibuffer))) > 0)
			{
				ibuffer[got-1] = '\0'; // get rid of '\n'
				printf("Reply from %s received: %s\n",
				inet_ntoa(rplyFromAddress.sin_addr), ibuffer);
			}
			close(newsock);
		}
		if (nfds == -1)
		{
			perror("select failed");
			exit(errno);
		}
		if (nfds == 0)
		{
			printf("Replies finished on timeout\n");
		}
		FD_SET(rplysock, & readfds);
		printf("Type your request: ");
	}
	printf("\n");
	close(rplysock);
	close(brsock);
	exit(0);
}
