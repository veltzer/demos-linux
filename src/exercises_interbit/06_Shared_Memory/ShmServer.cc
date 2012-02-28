#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

const int MAXCLIENTS=10;
const int CLIENTMESSAGESIZE=4096;

int main(int argc,char** argv,char** envp)
{
	struct data
	{
		int readOffset;
		int writeOffset;
		char message[CLIENTMESSAGESIZE];
	};
	struct data * smdata;
	int shmid;
	int semid;
	key_t key;
	int i;
	char ans[10];

	if ((key = ftok("/etc/passwd", 'x')) == -1)
	{
		perror("ftok failed");
		exit(errno);
	}
	if ((semid = semget(key, MAXCLIENTS, IPC_CREAT | 0666)) < 0 )
	{
		perror("semget");
		exit(1);
	}
	for(i=0; i<MAXCLIENTS; i++)
	{
		if (semctl(semid, i, SETVAL, 0) < 0)
		{
			perror("semctl 0");
			exit(errno);
		}
	}
	printf("asking for %d bytes\n", sizeof(struct data) * MAXCLIENTS);
	if((shmid = shmget(key, sizeof(struct data) * MAXCLIENTS, 
		IPC_CREAT | 0666)) < 0) 
	{
		perror("shmget failed");
		exit(errno);
	}
	if((smdata = (struct data *)shmat(shmid, NULL, 0)) == (struct data *) -1) 
	{
		perror("shmat failed");
		exit(errno);
	}
	for (i=0; i<MAXCLIENTS; i++)
	{
		smdata[i].readOffset = 0;
		smdata[i].writeOffset = 0;
	}
	printf("Hit <Enter> to finish\n");
	char* res=fgets(ans, sizeof(ans), stdin);
	if(res!=ans) {
		perror("fgets failed");
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		perror("shmctl IPC_RMID failed");
		exit(errno);
	}
	if (semctl(semid, 0, IPC_RMID, 0) == -1)
	{
		perror("semctl IPC_RMID failed");
		exit(errno);
	}
	return 0;
}
