#include <errno.h>

#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int NPHIL=5;
const char* KEYFILE="/etc/passwd";

union semun {
	int val;		/* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT, IPC_SET */
	unsigned short *array;	/* array for GETALL, SETALL */
				/* Linux specific part: */
	struct seminfo *__buf;	/* buffer for IPC_INFO */
};
