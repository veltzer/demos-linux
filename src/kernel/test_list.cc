#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#include "us_helper.hh"

#include "shared.h"

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
	int selection=-1;
	while(selection<0 || selection >6) {
		printf("What would you like to do ?\n");
		printf("===========================\n");
		printf("0) create\n");
		printf("1) destroy\n");
		printf("2) isempty\n");
		printf("3) add\n");
		printf("4) del\n");
		printf("5) print\n");
		printf("6) exit\n");
		printf("choice> ");
		char str[256];
		char *res = fgets(str, 256, stdin);
		if (res == NULL) {
			perror("problem with fgets");
			exit(1);
		}
		selection=atoi(str);
	}
	return selection;
}

static int codes[]={
	IOCTL_LIST_CREATE,
	IOCTL_LIST_DESTROY,
	IOCTL_LIST_ISEMPTY,
	IOCTL_LIST_ADD,
	IOCTL_LIST_DEL,
	IOCTL_LIST_PRINT,
};

int main(int argc, char **argv, char **envp) {
	const char *filename="/dev/mod_list";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_list");
	my_system("sudo insmod ./mod_list.ko");
	my_system("sudo chmod 666 %s",filename);
	int fd;
	sc(fd = open(filename, O_RDWR));
	int choice = show_menu();
	while (choice != 6) {
		int arg = 0;
		int ioctl_code=codes[choice];
		if (ioctl_code == IOCTL_LIST_ADD) {
			arg = get_number();
		}
		klog_clear();
		sc(ioctl(fd,ioctl_code,arg));
		klog_show_clear();
		//waitkey(NULL);
		choice = show_menu();
	}
	sc(close(fd));
	waitkey(NULL);
	return(0);
}
