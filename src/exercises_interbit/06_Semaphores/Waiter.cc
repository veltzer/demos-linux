#include"Phil.hh"

int semid;

void clean(int sig)
{
	int rc=semctl(semid,0,IPC_RMID,0);
	if(rc) {
		perror("error in semctl");
		exit(errno);
	} else {
		exit(0);
	}
}

int main(int argc,char** argv,char** envp)
{
	int i;
	key_t key;
	signal(SIGINT,clean);

	if ((key = ftok(KEYFILE, 'x')) == -1)
	{
		perror("ftok failed");
		exit(errno);
	}

	if ((semid = semget(key, NPHIL, IPC_CREAT | 0666)) < 0 )
	{
		perror("semget");
		exit(errno);
	}
	
	for(i=0; i<NPHIL; i++)
	{
		if (semctl(semid, i,SETVAL,1 ) < 0)
		{
			perror("semctl 1");
			exit(errno);
		}
	}
	while(1)
		pause();
	exit(0);
}
