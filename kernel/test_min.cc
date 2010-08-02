#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "us_helper.hh"

/*
 *      The most minimal test
 */
int main(int argc, char **argv, char **envp) {
	DEBUG("hello");
	return(0);
}
