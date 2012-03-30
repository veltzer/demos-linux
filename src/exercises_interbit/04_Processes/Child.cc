#include<errno.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv,char** envp)
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s 1 or 2\n", argv[0]);
		exit(1);
	}
	int me = atoi(argv[1]);
	if (me < 0 || me > 2)
	{
		fprintf(stderr, "I said 1 or 2\n");
		exit(1);
	}
	int sigme=0;
	switch (me)
	{
		case 1:
			sigme = SIGUSR1;
			break;
		case 2:
			sigme = SIGUSR2;
			break;
	}
	while(1)
	{
		sleep(2);
		kill(getppid(),sigme);
	}
	return 0;
}
