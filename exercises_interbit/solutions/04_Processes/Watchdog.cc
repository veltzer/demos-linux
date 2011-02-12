#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct itimerval timer;
int gotusr1=0; 
int gotusr2=0;
int child1pid;
int child2pid;

void sigintHandler(int gotsig)
{
	kill(child1pid, SIGKILL);
	kill(child2pid, SIGKILL);
	exit(0);
}

void sigchildHandler(int gotsig)
{
	pid_t pid;
	int status;
	while((pid = wait3(&status, WNOHANG, NULL)) > 0)
	{
		if(WIFEXITED(status))
			printf("Child %d exited. Status: %d\n",pid,WEXITSTATUS(status));
		if(WIFSIGNALED(status))
			printf("Child %d killed. Signal: %d\n",pid,WTERMSIG(status));
	}
}

void watchsigHandler(int gotsig)
{
	time_t now=time(NULL);
	char* stime=ctime(&now);
	stime[strlen(stime)-1]='\0';
	switch (gotsig)
	{
		case SIGUSR1:
			gotusr1 = 1;
			printf("%s: TTL from 1\n", stime);
			break;
		case SIGUSR2:
			gotusr2 = 1;
			printf("%s: TTL from 2\n", stime);
			break;
	}
	if (gotusr1 && gotusr2)
	{
		if (setitimer(ITIMER_REAL, & timer, NULL) == -1)
		{
			perror("setitimer failed");
			exit(errno);
		}
		gotusr1 = gotusr2 = 0;
	}
}

void doChild1()
{
	execl("./child", "./child", "1", NULL);
	perror("execl child1 failed");
	exit(errno);
}

void doChild2()
{
	execl("./child", "./child", "2", NULL);
	perror("execl child2 failed");
	exit(errno);
}

void startChild1()
{
	switch (child1pid = fork())
	{
		case -1:
			perror("fork for child1 failed");
			exit(errno);
			break;
		case 0:
			doChild1();
			exit(0);
			break;
	}
}

void startChild2()
{
	switch (child2pid = fork())
	{
		case -1:
			perror("fork for child2 failed");
			exit(errno);
			break;
		case 0:
			doChild2();
			exit(0);
			break;
	}
}

void timeoutsigHandler(int gotsig)
{
	if (gotusr1 == 0)
	{
		printf("Signal form child 1 ID %d lost. Restarting\n", child1pid);
		kill(child1pid, SIGKILL);
		startChild1();
	}
	if (gotusr2 == 0)
	{
		printf("Signal from child 2 ID %d lost. Restarting\n", child2pid);
		kill(child2pid, SIGKILL);
		startChild2();
	}
}

int main(int argc,char** argv,char** envp)
{
	struct sigaction sigusr, sigchld, sigalrm, sigint;
	sigset_t emptyset;
	sigemptyset(&emptyset);
	sigusr.sa_handler = watchsigHandler;
	sigusr.sa_mask = emptyset;
	sigusr.sa_flags = 0;

	if (sigaction(SIGUSR1, & sigusr, NULL) == -1)
	{
		perror("sigaction SIGUSR1 failed");
		exit(errno);
	}
	if (sigaction(SIGUSR2, & sigusr, NULL) == -1)
	{
		perror("sigaction SIGUSR2 failed");
		exit(errno);
	}
	sigchld.sa_handler = sigchildHandler;
	sigchld.sa_mask = emptyset;
	sigchld.sa_flags = 0;
	if (sigaction(SIGCHLD, & sigchld, NULL) == -1)
	{
		perror("sigaction SIGCHLD failed");
		exit(errno);
	}
	sigalrm.sa_handler = timeoutsigHandler;
	sigalrm.sa_mask = emptyset;
	sigalrm.sa_flags = 0;
	if (sigaction(SIGALRM, & sigalrm, NULL) == -1)
	{
		perror("sigaction SIGALRM failed");
		exit(errno);
	}
	sigint.sa_handler = sigintHandler;
	sigint.sa_mask = emptyset;
	sigint.sa_flags = 0;
	if (sigaction(SIGINT, & sigint, NULL) == -1)
	{
		perror("sigaction SIGINT failed");
		exit(errno);
	}
	timer.it_interval.tv_sec = 5;
	timer.it_interval.tv_usec = 0;
	timer.it_value.tv_sec = 5;
	timer.it_value.tv_usec = 0;
	if (setitimer(ITIMER_REAL, & timer, NULL) == -1)
	{
		perror("setitimer failed");
		exit(errno);
	}
	startChild1();
	startChild2();
	while (1)
	{
		pause();
	}
	return 0;
}
