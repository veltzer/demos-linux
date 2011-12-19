#include <stdio.h> // for printf(3), scanf(3)
#include <string.h> // for strlen(3)

#define STRINGSIZE 30
int main() {
	char mystring[STRINGSIZE];
	printf("Give me a string to check: ");
	//scanf("%s",mystring);
	char* ret=fgets(mystring,STRINGSIZE,stdin);
	if(ret==NULL) {
		perror("error in fgets");
		return -1;
	}
	int len=strlen(mystring);
	printf("len is %d\n",len);
	int palindrom=1;
	for(int i=0;i<len;i++) {
		if(mystring[i]!=mystring[len-i-1]) {
			palindrom=0;
			break;
		}
	}
	if(palindrom) {
		printf("YESS!!!!! string IS a palindrom...\n");
	} else {
		printf("bummer, string is not a palindrom...\n");
	}
	return 0;
}
