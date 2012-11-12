/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, exit(3)
#include<us_helper.h> // for CHECK_NOT_M1(), CHECK_NOT_VOIDP()

const int CLIENTMESSAGESIZE=1024;
const int MAXMESSAGE=256;
const int MAXCLINTS=10;

struct data {
	int readOffset;
	int writeOffset;
	char message[CLIENTMESSAGESIZE];
};

void doChild(int semid, struct data * smdata, int myID) {
	char message[MAXMESSAGE];
	int* messageSizePTR;
	int* fromIDPTR;
	int messageSize;
	int spaceLeft;
	struct sembuf sops[2];
	while(true) {
		sops[0].sem_num=myID;
		sops[0].sem_op=-1;
		sops[0].sem_flg=0;
		CHECK_NOT_M1(semop(semid, sops, 1));

		if (smdata[myID].readOffset!=smdata[myID].writeOffset) {
			messageSizePTR=(int *)smdata[myID].message + smdata[myID].readOffset/sizeof(int);
			messageSize=* messageSizePTR;
			spaceLeft=CLIENTMESSAGESIZE - smdata[myID].readOffset;
			if (spaceLeft < messageSize) {
				memcpy(message, smdata[myID].message + smdata[myID].readOffset, spaceLeft);
				memcpy(message+spaceLeft, smdata[myID].message, messageSize - spaceLeft);
				smdata[myID].readOffset=messageSize - spaceLeft;
			} else {
				memcpy(message, smdata[myID].message + smdata[myID].readOffset, messageSize);
				smdata[myID].readOffset+=messageSize;
			}
			fromIDPTR=(int *)message + 1;
			printf("\n\nHey! this is the child\nI got message from: %d message: %s\n", * fromIDPTR, message+2*sizeof(int));
		}
	}
}

void doParent(int semid, struct data * smdata, int myID) {
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
			exit(EXIT_FAILURE);
		}
		toID=atoi(Sto);
		printf("Message to %d: ", toID);

		s=fgets(message+sizeof(int)*2, sizeof(message) - sizeof(int)*2 -1, stdin);
		if(s!=message+sizeof(int)*2) {
			perror("fgets error");
			exit(EXIT_FAILURE);
		}

		messageSizePTR=(int *)message;
		* messageSizePTR=strlen(message+sizeof(int)*2) + sizeof(int)*2;

		FromIDPTR=(int *)message+1;
		* FromIDPTR=myID;

		reminder=* messageSizePTR % sizeof(int);
		if (reminder > 0)
			*messageSizePTR+=sizeof(int)-reminder;

		sops[0].sem_num=toID;
		sops[0].sem_op=1;
		sops[0].sem_flg=0;
		CHECK_NOT_M1(semop(semid,sops,1));

		spaceLeft=CLIENTMESSAGESIZE - smdata[toID].writeOffset;
		if (* messageSizePTR > spaceLeft) {
			memcpy(smdata[toID].message + smdata[toID].writeOffset, message, spaceLeft);
			memcpy(smdata[toID].message, message + spaceLeft, * messageSizePTR - spaceLeft);
			smdata[toID].writeOffset=* messageSizePTR - spaceLeft;
		} else {
			memcpy(smdata[toID].message + smdata[toID].writeOffset, message, * messageSizePTR);
			smdata[toID].writeOffset+=*messageSizePTR;
		}
	}
}

int main(int argc,char** argv,char** envp) {
	struct data * smdata;
	int shmid;
	int semid;
	key_t key;
	int myID;
	if(argc < 2) {
		fprintf(stderr, "Usage: %s MyID\n", argv[0]);
		exit(errno);
	}
	myID=atoi(argv[1]);
	if(myID>=MAXCLINTS || myID<0) {
		fprintf(stderr, "MyID must be 0-%d\n", MAXCLINTS);
		exit(errno);
	}
	CHECK_NOT_M1(key=ftok("/etc/passwd", 'x'));
	CHECK_NOT_M1(semid=semget(key, 0, 0));
	CHECK_NOT_M1(shmid=shmget(key, sizeof(smdata), 0));
	CHECK_NOT_VOIDP(smdata=(struct data *)shmat(shmid, NULL, 0),(struct data *) -1);
	smdata[myID].readOffset=0;
	smdata[myID].writeOffset=0;
	int fres;
	CHECK_NOT_M1(fres=fork());
	if(fres==0) {
		doChild(semid, smdata, myID);
	} else {
		doParent(semid, smdata, myID);
	}
	return EXIT_SUCCESS;
}
