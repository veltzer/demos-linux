#include <stdio.h>
#include <unistd.h>
#include <err_utils.h>

int main() {
	FILE* f=fopen("myfile", "r");
	while(true) {
		char buf[1024];
		CHECK_NOT_NULL(fgets(buf, 1024, f));
		printf("yes!!! got [%s]\n", buf);
		sleep(1);
	}
	return 0;
}
