#include<sys/ioctl.h> // for ioctl(2)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
 * This is a test to check my own us_helper.hh file.
 */

int main(int argc, char **argv, char **envp) {
	CHECK_NOT_M1(ioctl(5,7));
	return EXIT_SUCCESS;
}
