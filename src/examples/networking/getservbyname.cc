#include <netdb.h> // for getservbyname(3)
#include <stdio.h> // for printf(3)

#include "us_helper.hh" // our own helper

/*
 * A simple example to demonstrate getservbyname(3)
 *
 * 			Mark Veltzer
 */
int main(int argc, char** argv,char** envp) {
	if(argc!=3) {
		printf("usage: %s [service-name] [protocol-name]\n",argv[0]);
		return EXIT_FAILURE;
	}
	// getservbyname() - opens the etc.services file and returns the
	// values for the requested service and protocol.
	struct servent *serv;
	CHECK_NOT_NULL(serv=getservbyname(argv[1], argv[2]));
	// Print it.
	printf("Name: %s Port: %d Protocol: %s\n",serv->s_name,ntohs(serv->s_port),serv->s_proto);
	return EXIT_SUCCESS;
}
