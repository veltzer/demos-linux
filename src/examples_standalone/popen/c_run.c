#include<stdio.h> // for fopen(3), fprintf(3), fclose(3), getline(3), popen(3), pclose(3)
#include<stdlib.h> // for system(3), abort(3), mkstemp(3), malloc(3)
#include<unistd.h> // for close(2)

//#define DEBUG

void my_system(const char* cmd) {
#ifdef DEBUG
	fprintf(stderr,"doing system [%s]\n",cmd);
#endif // DEBUG
	int ret=system(cmd);
	if(ret==-1) {
		perror("system failed");
		abort();
	}
}

/*
 * cut all but the first line in the file and return the temp file holding the result
 */
void cut_first_line(const char* filename,FILE* output) {
	FILE* input=fopen(filename,"r");
	if(input==NULL) {
		perror("cannot open file");
		abort();
	}
	size_t n=1024;
	char* lineptr=(char*)malloc(n);
	int ret;
	int active=0;
	while((ret=getline(&lineptr,&n,input)>0)) {
		if(active) {
#ifdef DEBUG
			fprintf(stderr,"writing out [%s]\n",lineptr);
#endif // DEBUG
			int ret=fputs(lineptr,output);
			if(ret==EOF) {
				perror("could not fputs");
				abort();
			}
		} else {
			active=1;
		}
	}
	if(ret!=0) {
		fprintf(stderr,"got bad value from getline(3)");
		abort();
	}
	if(fclose(input)==EOF) {
		perror("got bad value from fclose(3)");
		abort();
	}
}

static char template[]="/tmp/crun.XXXXXX";
int main(int argc,char** argv,char** envp) {
	// handle command line
	if(argc!=2) {
		fprintf(stderr,"usage: %s [cshell file]\n",argv[0]);
		abort();
	}
	const char* filename=argv[1];
	// create a filename for the executable
	int fd=mkstemp(template);
	if(fd==-1) {
		perror("got bad value from mkstemp(3)");
		abort();
	}
#ifdef DEBUG
	fprintf(stderr,"template is [%s]\n",template);
#endif // DEBUG
	// run the compiler until eof...
	const int max=1024;
	char cmd[max];
	sprintf(cmd,"gcc -x c -o %s -O2 -",template);
#ifdef DEBUG
	fprintf(stderr,"cmd is [%s]\n",cmd);
#endif // DEBUG
	FILE* handle=popen(cmd,"w");
	if(handle==NULL) {
		perror("error with popen");
		abort();
	}
	cut_first_line(filename,handle);
	int ret=pclose(handle);
	if(ret==-1) {
		perror("got bad value from pclose(3)");
		abort();
	}
	// close the template
	ret=close(fd);
	if(ret==-1) {
		perror("got bad value from close(2)");
		abort();
	}
	// now run the resulting executable
	my_system(template);
	// finally unlink the template
	ret=unlink(template);
	if(ret==-1) {
		perror("got bad value from unlink(2)");
		abort();
	}
	return(0);
}
