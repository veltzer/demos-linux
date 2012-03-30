#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int CLIENTMESSAGESIZE=1024;
const int MAXMESSAGE=256;
const int MAXCLINTS=10;

struct data
{
	int readOffset;
	int writeOffset;
	char message[CLIENTMESSAGESIZE];
};

void doChild(int semid, struct data * smdata, int myID)
{
	char message[MAXMESSAGE];
	int * messageSizePTR;
	int * fromIDPTR;
	int messageSize;
	int spaceLeft;
	struct sembuf sops[2];
	while(1)
	{
		sops[0].sem_num = myID;
		sops[0].sem_op = -1;
		sops[0].sem_flg = 0;
		if ( semop(semid, sops, 1) == -1 )
		{
			perror("semop");
			exit(1);
		}
		
		if (smdata[myID].readOffset != smdata[myID].writeOffset)
		{
			messageSizePTR = (int *)smdata[myID].message + smdata[myID].readOffset/sizeof(int);
			messageSize = * messageSizePTR;
			spaceLeft = CLIENTMESSAGESIZE - smdata[myID].readOffset;
			if (spaceLeft < messageSize)
			{
				memcpy(message, smdata[myID].message + smdata[myID].readOffset, spaceLeft);
				memcpy(message+spaceLeft, smdata[myID].message, messageSize - spaceLeft);
				smdata[myID].readOffset = messageSize - spaceLeft;
			}
			else
			{
				memcpy(message, smdata[myID].message + smdata[myID].readOffset, messageSize);
				smdata[myID].readOffset += messageSize;
			}
			fromIDPTR = (int *)message + 1;
			printf("From: %d message: %s\n", * fromIDPTR, message+2*sizeof(int));
		}
	}
}

void doParent(int semid, struct data * smdata, int myID)
{
	char message[MAXMESSAGE];
	int * messageSizePTR;
	int * FromIDPTR;
	char Sto[8];
	int toID;
	int reminder;
	int spaceLeft;
	struct sembuf sops[2];
	while(1)
	{
		printf("To: ");
		char* s=fgets(Sto, 7, stdin);
		if(s!=Sto) {
			perror("fgets error");
		}
		toID = atoi(Sto);
		printf("Message to %d: ", toID);
	
		s=fgets(message+sizeof(int)*2, sizeof(message) - sizeof(int)*2 -1, stdin);
		if(s!=message) {
			perror("fgets error");
		}
		
		messageSizePTR = (int *)message;
		* messageSizePTR = strlen(message+sizeof(int)*2) + sizeof(int)*2;
		
		FromIDPTR = (int *)message+1;
		* FromIDPTR = myID;
		
		reminder = * messageSizePTR % sizeof(int);
		if (reminder > 0)
			* messageSizePTR += sizeof(int) - reminder;
		sops[0].sem_num = toID;
		sops[0].sem_op = 1;
		sops[0].sem_flg = 0;
		
		if ( semop(semid, sops, 1) == -1 )
		{
			perror("semop");
			exit(1);
		}
		
		spaceLeft = CLIENTMESSAGESIZE - smdata[toID].writeOffset;
		if (* messageSizePTR > spaceLeft)
		{
			memcpy(smdata[toID].message + smdata[toID].writeOffset, message, spaceLeft);
			memcpy(smdata[toID].message, message + spaceLeft, * messageSizePTR - spaceLeft);
			smdata[toID].writeOffset = * messageSizePTR - spaceLeft;
		}
		else
		{
			memcpy(smdata[toID].message + smdata[toID].writeOffset, message, * messageSizePTR);
			smdata[toID].writeOffset += * messageSizePTR;
		}
	}
}

int main(int argc,char** argv,char** envp)
{
	struct data * smdata;
	int shmid;
	int semid;
	key_t key;
	int myID;
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s MyID\n", argv[0]);
		exit(errno);
	}
	myID = atoi(argv[1]);
	if (myID >= MAXCLINTS || myID < 0)
	{
		fprintf(stderr, "MyID must be 0-%d\n", MAXCLINTS);
		exit(errno);
	}
	if ((key = ftok("/etc/passwd", 'x')) == -1)
	{
		perror("ftok failed");
		exit(errno);
	}
	if ((semid = semget(key, 0, 0)) == -1)
	{
		perror("semget failed");
		exit(errno);
	}
	if ((shmid = shmget(key, sizeof(smdata), 0)) < 0) 
	{
		perror("shmget failed");
		exit(errno);
	}
	if ((smdata = (struct data *)shmat(shmid, NULL, 0)) == (struct data *) -1) 
	{
		perror("shmat failed");
		exit(errno);
	}
	smdata[myID].readOffset = 0;
	smdata[myID].writeOffset = 0;
	switch (fork())
	{
		case -1:
			perror("fork failed");
			exit(errno);
		case 0:
			doChild(semid, smdata, myID);
			exit(0);
		default:
			doParent(semid, smdata, myID);
			exit(0);
	}
	return 0;
}
