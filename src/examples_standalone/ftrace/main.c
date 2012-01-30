#include <stdio.h>

int call_func(int a, int b) {
	return(a + b);
}


int main(int argc, char **argv, char **envp) {
	int res;

	res = call_func(1, 1);
	return(0);
}
