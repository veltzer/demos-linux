
int add(int a,int b) {
	return a+b;
}

int main(int argc,char** argv,char** envp) {
	char* p=(char*)add;
	*p=7;
	return 0;
}
