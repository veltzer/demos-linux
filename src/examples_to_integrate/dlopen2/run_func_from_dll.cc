#include<stdio.h>
#include<dlfcn.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc,char** argv,char** envp) {
	if(argc!=4) {
		fprintf(stderr,"%s: usage: %s [library] [function] [value]\n",argv[0],argv[0]);
		exit(-1);
	}
	const char* p_lib=argv[1];
	const char* p_func=argv[2];
	const char* p_sval=argv[3];
	const double p_dval=atof(p_sval);
	void* h=dlopen(p_lib,RTLD_NOW);
	if(h==NULL) {
		fprintf(stderr,"error with dlopen: %s\n",dlerror());
		exit(errno);
	}
	void* sym=dlsym(h,p_func);
	if(sym==NULL) {
		fprintf(stderr,"error with dlsym: %s\n",dlerror());
		exit(errno);
	}
	double (*f)(double)=(typeof(f))sym;
	double result=f(p_dval);
	printf("result is %lf\n",result);
	int rc=dlclose(h);
	if(rc) {
		fprintf(stderr,"error with dlclose: %s\n",dlerror());
		exit(errno);
	}
	return(0);
}
