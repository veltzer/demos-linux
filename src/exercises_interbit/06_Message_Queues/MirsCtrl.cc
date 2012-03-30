#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

const int MSGSZ=128;
const int MAXQUEUE=32768;

typedef struct mymsgbuf 
{
	long mtype;
	long fromID;
	char mtext[MSGSZ];
} message_buf;

int childPID;
int msqid;
struct msqid_ds msgCtlBuf;

void TerminateChild(int gotsig)
{
	int rc=kill(childPID, SIGKILL);
	if(rc) {
		perror("error in kill");
		exit(errno);
	} else {
		exit(0);
	}
}

void DoChild()
{
	int msgsize;
	int currentQueueSize, biggestQueueSize;
	int i, biggestQID=0;
	message_buf rbuf;
	while(1)
	{
		if (msgctl(msqid, IPC_STAT, & msgCtlBuf) == -1)
		{
			perror("msgctl: msgctl failed");
			exit(errno);
		}
		if(msgCtlBuf.msg_cbytes + MSGSZ+sizeof(long) >= msgCtlBuf.msg_qbytes)
		{
			// try to empty all queues. find the one with most messages and report it.
			biggestQueueSize = 0;
			for(i=1; i<MAXQUEUE; i++)
			{
				currentQueueSize = 0;
				while ((msgsize = msgrcv(msqid, &rbuf, MSGSZ+sizeof(long), 
					i, IPC_NOWAIT|MSG_NOERROR))!=-1)
				{
					currentQueueSize += msgsize;
				}
				if (currentQueueSize > biggestQueueSize)
				{
					biggestQueueSize = currentQueueSize;
					biggestQID = i;
				}
			}
			printf("\n\nAll Queues cleared.\nBiggest queue was %d containing %d bytes.", 
				biggestQID, biggestQueueSize);
		}
		sleep(5);
	}
}

int main(int argc,char** argv,char** envp)
{
	key_t key;
	struct sigaction SigAction;
	sigset_t emptymask;
	int rc;

	sigemptyset(&emptymask);
	SigAction.sa_handler = TerminateChild;
	SigAction.sa_mask = emptymask;
	SigAction.sa_flags = 0;
	if (sigaction(SIGINT, &SigAction, NULL) == -1)
	{
		perror("sigaction");
		exit(errno);
	}
	if (sigaction(SIGTERM, &SigAction, NULL) == -1)
	{
		perror("sigaction");
		exit(errno);
	}
	if (sigaction(SIGQUIT, &SigAction, NULL) == -1)
	{
		perror("sigaction");
		exit(errno);
	}
	if (sigaction(SIGABRT, &SigAction, NULL) == -1)
	{
		perror("sigaction");
		exit(errno);
	}
	if ((key = ftok("MirsClient", 'x')) == -1)
	{
		perror("ftok failed");
		exit(errno);
	}
	if ((msqid = msgget(key, IPC_CREAT | 0666 )) < 0)
	{
		perror("msgget failed");
		exit(errno);
	}
	switch (childPID = fork())
	{
		case -1:
			perror("fork failed");
			exit(errno);
		case 0:
			DoChild();
			exit(0);
	}
	while(1)
	{
		int Ans;
		printf("\n\n\n\n\nMain Menu\n\n");
		printf("What would you like to do?\n");
		printf("1. Show all the queue properties\n");
		printf("2. Stop the service (it's a strike).\n");
		printf("3. Change queue size\n");
		printf("4. Change permission\n -> ");
		int ret=scanf("%d", & Ans);
		if(ret<1) {
			perror("fscanf(3)");
		}
		switch (Ans)
		{
			case 1:
				if(msgctl(msqid, IPC_STAT, & msgCtlBuf) == -1)
				{
					perror("msgctl: msgctl failed");
					exit(errno);
				}
				fprintf(stderr, "msg_perm.uid = %d\n", msgCtlBuf.msg_perm.uid);
				fprintf(stderr, "msg_perm.gid = %d\n", msgCtlBuf.msg_perm.gid);
				fprintf(stderr, "msg_perm.cuid = %d\n", msgCtlBuf.msg_perm.cuid);
				fprintf(stderr, "msg_perm.cgid = %d\n", msgCtlBuf.msg_perm.cgid);
				fprintf(stderr, "msg_perm.mode = %#o, ", msgCtlBuf.msg_perm.mode);
				fprintf(stderr, "access permissions = %#o\n", msgCtlBuf.msg_perm.mode & 0777);
				fprintf(stderr, "msg_cbytes = %ld\n", msgCtlBuf .msg_cbytes);
				fprintf(stderr, "msg_qbytes = %ld\n", msgCtlBuf.msg_qbytes);
				fprintf(stderr, "msg_qnum = %ld\n", msgCtlBuf.msg_qnum);
				fprintf(stderr, "msg_lspid = %d\n", msgCtlBuf.msg_lspid);
				fprintf(stderr, "msg_lrpid = %d\n", msgCtlBuf.msg_lrpid);
				fprintf(stderr, "msg_stime = %s", msgCtlBuf.msg_stime ?
					ctime(&msgCtlBuf.msg_stime) : "Not Set\n");
				fprintf(stderr, "msg_rtime = %s", msgCtlBuf.msg_rtime ?
					ctime(&msgCtlBuf.msg_rtime) : "Not Set\n");
				fprintf(stderr, "msg_ctime = %s", ctime(&msgCtlBuf.msg_ctime));
				break;
			case 2:
				rc=kill(childPID, SIGKILL);
				if(rc) {
					perror("error in kill");
					exit(errno);
				}
				if (msgctl(msqid, IPC_RMID, NULL) == -1)
				{
					perror("msgctl RMID failed");
					exit(errno);
				}
				exit(0);
				break;
			case 3:
				if (msgctl(msqid, IPC_STAT, & msgCtlBuf) == -1)
				{
					perror("msgctl: msgctl failed");
					exit(errno);
				}
				printf("From %ld\nTo ",msgCtlBuf.msg_qbytes);
				ret=scanf("%ld", & msgCtlBuf.msg_qbytes);
				if(ret<1) {
					perror("scanf");
				}
				if (msgctl(msqid, IPC_SET, & msgCtlBuf) == -1)
				{
					perror("msgctl: msgctl failed");
					exit(errno);
				}
				break;
			case 4:
				fprintf(stderr, "Enter msg_perm.uid: ");
				ret=scanf ("%d", &msgCtlBuf.msg_perm.uid);
				if(ret<1) {
					perror("scanf");
				}
				fprintf(stderr, "Enter msg_perm.gid: ");
				ret=scanf("%d", &msgCtlBuf.msg_perm.gid);
				if(ret<1) {
					perror("scanf");
				}
				fprintf(stderr, "Enter msg_perm.mode: ");
				ret=scanf("%hd", &msgCtlBuf.msg_perm.mode); 
				if(ret<1) {
					perror("scanf");
				}
				if (msgctl(msqid, IPC_SET, & msgCtlBuf) == -1)
				{
					perror("msgctl: msgctl failed");
					exit(errno);
				}
		}
	}
	return 0;
}
