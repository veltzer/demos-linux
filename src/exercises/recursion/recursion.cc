#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_STR_LEN 64

void maximum_subset(char str[], int index, char curr[], char best[]) {
	int length_of_curr, length_of_best;
	char tmp_curr[MAX_STR_LEN];
	int tmp_length_of_curr;
	/* End of the recursion */
	//printf("called with [%s, %d, %s, %s]\n", str, index, curr, best);
	if(str[index]=='\0') {
		return;
	}
	length_of_curr=strlen(curr);
	length_of_best=strlen(best);
	/* Use the current character as an extension of current or as new string */
	strcpy(tmp_curr, curr);
	tmp_length_of_curr=length_of_curr;
	if(!(length_of_curr>0 && str[index]>curr[length_of_curr-1])) {
		length_of_curr=0;
	}
	curr[length_of_curr]=str[index];
	length_of_curr+=1;
	curr[length_of_curr]='\0';
	if(length_of_curr>length_of_best) {
		strcpy(best, curr);
		length_of_best=length_of_curr;
	}
	maximum_subset(str, index+1, curr, best);
	length_of_best=strlen(best);
	strcpy(curr, tmp_curr);
	length_of_curr=tmp_length_of_curr;
	/* skip the current character */
	maximum_subset(str, index+1, curr,  best);
}

void check(const char* val, const char* expected) {
	char str[MAX_STR_LEN];
	char curr[MAX_STR_LEN]="";
	char best[MAX_STR_LEN]="";
	strcpy(str, val);
	maximum_subset(str, 0, curr, best);
	printf("[%s] - [%s] - ", val, expected);
	if(strcmp(expected, best)!=0) {
		printf("ERROR ([%s])\n", best);
	} else {
		printf("OK\n");
	}
}

int main(int argc,char** argv, char** envp) {
	check("", "");
	check("a", "a");
	check("abc", "abc");
	check("azbcd", "abcd");
	check("abcabcd", "abcd");
	check("avxwzbcdef", "abcdef");
	check("cbe", "ce");
	check("nicbeart", "cert");
	return EXIT_SUCCESS;
}
