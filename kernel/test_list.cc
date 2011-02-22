#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#include "us_helper.hh"

/*
 *      this is a test for in kernel lists
 */
int get_number(void) {
	printf("please enter a number> ");
	char str[256];
	char *res = fgets(str, 256, stdin);
	if (res == NULL) {
		perror("problem with fgets");
		exit(1);
	}
	return(atoi(str));
}


int show_menu(void) {
	printf("What would you like to do ?\n");
	printf("===========================\n");
	printf("0) create\n");
	printf("1) destroy\n");
	printf("2) isempty\n");
	printf("3) pop\n");
	printf("4) add\n");
	printf("5) print\n");
	printf("6) exit\n");
	printf("choice> ");
	char str[256];
	char *res = fgets(str, 256, stdin);
	if (res == NULL) {
		perror("problem with fgets");
		exit(1);
	}
	return(atoi(str));
}


int main(int argc, char **argv, char **envp) {
	const char *filename = "/dev/demo";
	int d;
	int arg;

	SC(d = open(filename, O_RDWR));
	int choice = show_menu();
	while (choice != 6) {
		printf("your choice is %d> \n", choice);
		arg = 0;
		if (choice == 4) {
			arg = get_number();
		}
		klog_clear();
		SC(ioctl(d, choice, arg));
		klog_show();
		waitkey();
		choice = show_menu();
	}
	SC(close(d));
	waitkey();
	return(0);
}
