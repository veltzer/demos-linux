#include <stdlib.h> // for abort(3)
#include <stdio.h> // for printf(3)
#include <mcheck.h> // for mcheck(3), states, mprobe(3)
#include <string.h> // for strcpy(3)

void myhandler(enum mcheck_status status) {
	printf("inside my handler, careful not to do something complicated here since the system is in a bad state as it is...\n");
	char state[256];
	switch(status) {
		case MCHECK_DISABLED:
			strcpy(state,"disabled");
			break;
		case MCHECK_OK:
			strcpy(state,"ok");
			break;
		case MCHECK_HEAD:
			strcpy(state,"head");
			break;
		case MCHECK_TAIL:
			strcpy(state,"tail");
			break;
		case MCHECK_FREE:
			strcpy(state,"free");
			break;
	}
	printf("state is %s\n",state);
	abort();
}

int main(int argc,char** argv,char** envp) {
	// I am using my own handler but the default one is ok (prints an error
	// message and aborts).
	// This call is neccessary if you havent linked with -lmcheck or if you really
	// want to use your own handler.
	mcheck(myhandler);
	char* buf=(char*)malloc(10);
	int i;
	for(i=0;i<1000;i++) {
		buf[i]=i%26+'a';
	}
	buf[1000]='\0';
	printf("buf is %s\n",buf);
	// check the status of our allocated chunk...
	int status=mprobe(buf);
	printf("You will not reach this line if the probe fails");
	printf("status is %d\n",status);
	return(0);
}
