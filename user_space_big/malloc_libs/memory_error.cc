#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for malloc(3), free(3), atoi(3)
#ifdef DMALLOC
#include <dmalloc.h>
#endif // DMALLOC

/*
 * This test program simply simulates various memory errors
 */

int main(int argc,char** argv,char** envp) {
	if(argc!=2) {
		fprintf(stderr,"usage: memory_error [0-5]\n");
		fprintf(stderr,"\t0 - write before the buffer and free\n");
		fprintf(stderr,"\t1 - write after the buffer and free\n");
		fprintf(stderr,"\t2 - write before the buffer and dont free\n");
		fprintf(stderr,"\t3 - write after the buffer and dont free\n");
		fprintf(stderr,"\t4 - read before the buffer\n");
		fprintf(stderr,"\t5 - read after the buffer\n");
		fprintf(stderr,"\t6 - forget to release the buffer\n");
		fprintf(stderr,"\t7 - release the buffer twice\n");
		return 1;
	}
	unsigned int type=atoi(argv[1]);
	const int size=100;
	fprintf(stderr,"starting\n");
	char* buffer=(char*)malloc(size);
	switch(type) {
		case 0:
			for(int i=0;i>-50;i--) {
				buffer[i]=i%256;
			}
			free(buffer);
			break;
		case 1:
			for(int i=0;i<size*2;i++) {
				fprintf(stderr,"i is %d\n",i);
				buffer[i]=i%256;
			}
			free(buffer);
			break;
		case 2:
			for(int i=0;i>-50;i--) {
				buffer[i]=i%256;
			}
			break;
		case 3:
			for(int i=0;i<size*2;i++) {
				fprintf(stderr,"i is %d\n",i);
				buffer[i]=i%256;
			}
			break;
		case 4:
			for(int i=0;i>-50;i--) {
				printf("value at %d is %c\n",i,buffer[i]);
			}
			break;
		case 5:
			for(int i=0;i<size*2;i++) {
				printf("value at %d is %c\n",i,buffer[i]);
			}
			break;
		case 6:
			// no need to do anything, just don't free the buffer
			break;
		case 7:
			free(buffer);
			free(buffer);
			break;
	}
	fprintf(stderr,"ending\n");
	return(0);
}
