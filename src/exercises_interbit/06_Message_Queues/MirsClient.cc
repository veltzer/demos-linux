#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int MSGSZ=1024;

// note we cannot declare msgbuf and get away with it
typedef struct mymsgbuf 
{
	long mtype;
	long fromID;
	char mtext[MSGSZ];
} message_buf;

void doParent(int msqid, long myID)
{
	char SsubscriberID[8];
	message_buf sbuf;
	setbuf(stdin,0);
	setbuf(stdout,0);
	while(1)
	{
		printf("Call ID: ");
		char* ret=fgets(SsubscriberID, sizeof(SsubscriberID)-1, stdin);
		if(ret!=SsubscriberID) {
			perror("fgets(3)");
		}
		sbuf.mtype = atoi(SsubscriberID);
		sbuf.fromID = myID;
		printf("Message to %ld: ", sbuf.mtype);
		ret=fgets(sbuf.mtext, sizeof(sbuf.mtext) -1, stdin);
		if(ret!=SsubscriberID) {
			perror("fgets(3)");
		}
		if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext)+sizeof(long), 0) == -1)
		{
			perror("msgsnd failed");
			exit(errno);
		}
	}
}

void doChild(int msqid, long myID)
{
	message_buf rbuf;
	int msgsize;
	while (1)
	{
		if ((msgsize = msgrcv(msqid, &rbuf, MSGSZ+sizeof(long), myID, 0)) == -1)
		{
			perror("msgrcv failed");
			exit(errno);
		}
		rbuf.mtext[msgsize-sizeof(long)] = '\0'; // string was sent without null char.
		printf("Message from: %ld -> %s", rbuf.fromID, rbuf.mtext);
	}
}

int main(int argc,char** argv,char** envp)
{
	int msqid;
	long myID;
	key_t key;
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s MirsID\n", argv[0]);
		exit(1);
	}
	if ((myID = atoi(argv[1])) < 1)
	{
		fprintf(stderr, "MirsID must be numeric positive greater than 0 and uniq (not checked)\n");
		exit(1);
	}
	if ((key = ftok("MirsClient", 'x')) == -1)
	{
		perror("ftok failed");
		exit(errno);
	}
	if ((msqid = msgget(key, 0)) < 0) 
	{
		perror("msgget failed");
		exit(errno);
	}
	switch (fork())
	{
		case -1:
			perror("Fork failed");
			exit(errno);
		case 0:
			doChild(msqid, myID);
			exit(0);
		default:
			doParent(msqid, myID);
			exit(0);
	}
	return 0;
}
