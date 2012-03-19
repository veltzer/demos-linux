#include <iostream> // for std::cout, std::endl
#include <string.h> // for memcpy(3)
#include <stdlib.h> // for malloc(3)

/*
 * This is a simple example that spends lots of time in a function.
 * You can watch this with 'perf top' and enter the function with 's'.
 *
 *		Mark Veltzer
 */

class A {
	public:
		static void performance_hog_function(char* buf1,char* buf2,unsigned int size) __attribute__((noinline)) {
		//static void performance_hog_function() {
			//float sum=0;
			for(unsigned int i=0;i<1000000;i++) {
				for(unsigned int j=0;j<1000000;j++) {
					memcpy(buf1,buf2,size);
					//sum+=i*j;
				}
			}
			//std::cout << sum << std::endl;

		}
};

int main(int argc, char **argv, char **envp) {
	const unsigned int size=1024*1024;
	char* buf1=(char*)malloc(size);
	char* buf2=(char*)malloc(size);
	A::performance_hog_function(buf1,buf2,size);
	return(0);
}
