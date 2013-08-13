#include <stdio.h>	// for fwrite(3), fflush(3), stdout
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for read(2)
#include <err_utils.h>	// for CHECK_NOT_M1()

int main(int argc, char** argv, char** envp) {
	int res;
	int fd=CHECK_NOT_M1(open("/dev/input/mouse0", O_RDONLY));
	char buf[1024];
	res=read(fd, buf, sizeof(buf));
	while(res!=0) {
		fwrite(buf, 1, res, stdout);
		fflush(stdout);
		res=read(fd, buf, sizeof(buf));
	}
	return EXIT_SUCCESS;
}
