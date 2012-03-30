#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/wait.h>

#include"us_helper.hh"

/*
 * This is a test which will explain which methods in the driver are
 * called. Here are the scenarios which are shown:
 * - a single process opening and closing.
 * - a pair of processes both opening and closing the same file.
 * - a process which open a file and dies.
 * - two processes one which dies.
 *
 * This example explains when open and release are called.
 * open - is called when a unique new fd is created based on the device driver file.
 *	It is NOT called when an already existing fd is duplicated like in the
 *	case of fork(2) or dup(2).
 * release - is called when the final fd of the last process is closed. This means
 *	that no userspace process still holds a connection to that fd.
 *	It is NOT called when a close(2) occurs which is not the last one...
 * Common questions are:
 * 1. how do I know within the driver that a certain process
 * that talked to my driver died?
 * 2. how do I know within the driver that a new process
 * was created that is using a file descriptor that I am serving?
 * 3. how do I know within the driver that a file descriptor
 * that I am serving just god dupped?
 * Some attempts at answers:
 * - you don't. file descriptors in UNIX are NOT tied to
 * one process - they may be shared. You only get notification when they
 * are created or finally destroyed.
 * - one strategy is to not share file descriptors. Close them in the parent or child if
 * you fork or only open them after forks.
 * - have every process which closes the descriptor do
 * an ioctl(2) on it before closing notifying you. The same could be done
 * after fork(2) or dup(2). But why would you want that?
 */

// file descriptor
int d, d2;
// file to be used
const char *filename = "/dev/mod_open_close_release";

void do_open_close(void) {
	int d;

	CHECK_NOT_M1(d = open(filename, O_RDWR));
	CHECK_NOT_M1(close(d));
}


void do_close(void) {
	CHECK_NOT_M1(close(d));
}


void do_open_and_forget(void) {
	int d;

	CHECK_NOT_M1(d = open(filename, O_RDWR));
}


void do_open_and_segfault(void) {
	int d;

	CHECK_NOT_M1(d = open(filename, O_RDWR));
	char *p = (char *)NULL;
	*p = 0;
}


pid_t run_in_process(void (*f)(void)) {
	pid_t pid;

	CHECK_NOT_M1(pid = fork());
	if (pid == 0) {
		f();
		exit(0);
	} else {
		return(pid);
	}
}


int main(int argc, char **argv, char **envp) {
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_open_close_release");
	my_system("sudo insmod ./mod_open_close_release.ko");
	my_system("sudo chmod 666 %s",filename);
	printf("Scenario 1 - a single process opening and closing...\n");
	printf("=============================================================\n");
	klog_clear();
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	CHECK_NOT_M1(close(d));
	klog_show();
	waitkey(NULL);
	printf("Scenario 2 - a single process opening twice and closing twice...\n");
	printf("=============================================================\n");
	klog_clear();
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	CHECK_NOT_M1(d2 = open(filename, O_RDWR));
	CHECK_NOT_M1(close(d));
	CHECK_NOT_M1(close(d2));
	klog_show();
	waitkey(NULL);
	printf("Scenario 3 - two process opening and closing the same file...\n");
	printf("=============================================================\n");
	klog_clear();
	pid_t c1 = run_in_process(do_open_close);
	pid_t c2 = run_in_process(do_open_close);
	int status;
	CHECK_NOT_M1(waitpid(c1, &status, 0));
	CHECK_NOT_M1(waitpid(c2, &status, 0));
	klog_show();
	waitkey(NULL);
	printf("Scenario 4 - parent opens, spawns two children who close and closes...\n");
	printf("=============================================================\n");
	klog_clear();
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	c1 = run_in_process(do_close);
	c2 = run_in_process(do_close);
	CHECK_NOT_M1(close(d));
	CHECK_NOT_M1(waitpid(c1, &status, 0));
	CHECK_NOT_M1(waitpid(c2, &status, 0));
	klog_show();
	waitkey(NULL);
	printf("Scenario 5 - open, dup and two closes...\n");
	printf("=============================================================\n");
	klog_clear();
	CHECK_NOT_M1(d = open(filename, O_RDWR));
	CHECK_NOT_M1(d2 = dup(d));
	CHECK_NOT_M1(close(d));
	CHECK_NOT_M1(close(d2));
	klog_show();
	waitkey(NULL);
	printf("Scenario 6 - open and forget to close\n");
	printf("=============================================================\n");
	klog_clear();
	c1 = run_in_process(do_open_and_forget);
	CHECK_NOT_M1(waitpid(c1, &status, 0));
	klog_show();
	waitkey(NULL);
	printf("Scenario 7 - open and segfault\n");
	printf("=============================================================\n");
	klog_clear();
	c1 = run_in_process(do_open_and_segfault);
	CHECK_NOT_M1(waitpid(c1, &status, 0));
	klog_show();
	waitkey(NULL);
	return(0);
}
