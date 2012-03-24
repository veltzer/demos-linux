#include <sys/types.h> // for socket(2)
#include <sys/socket.h> // for socket(2)
#include <stdio.h> // for printf(3), fprintf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <unistd.h> // for read(2)
#include <netinet/ip.h> // for struct iphdr
#include <netinet/tcp.h> // for struct tcphdr
#include <netinet/in.h> // for IPPROTO_TCP
#include <unistd.h> // for geteuid(2)
#include <sys/types.h> // for geteuid(2)

#include "us_helper.hh" // our own helper

/*
 * An example of a sniffer 
 *
 * References:
 * http://www.tenouk.com/Module42a.html
 *
 * 		Mark Veltzer
 */
 
int main(int argc, char** argv,char** envp) {
	// lets check that we are running as root
	uid_t euid=geteuid();
	if(euid!=0) {
		fprintf(stderr,"You should run as euid==0 (you are running as euid=%d)\n",euid);
		fprintf(stderr,"Do you want to try sudo?\n");
		return EXIT_FAILURE;
	}
	int fd;
	CHECK_NOT_M1(fd=socket(PF_INET, SOCK_RAW, IPPROTO_TCP));
	/* single packets are usually not bigger than 8192 bytes but
	 * depend on the media standard of the Network Access layer such as
	 * Ethernet, Token Ring etc
	 */
	const unsigned int length=8192;
	char buffer[length];
	//struct ipheader *ip = (struct ipheader *) buffer;
	//struct udpheader *udp = (struct udpheader *) (buffer + sizeof(struct ipheader));
	while (read(fd, buffer, length) > 0) {
		/* packet = data + ip header + tcp header */
		/* Little Endian/Big Endian must be considered here */
		 printf("Dump the packet: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct tcphdr));
	}
	return EXIT_SUCCESS;
}
