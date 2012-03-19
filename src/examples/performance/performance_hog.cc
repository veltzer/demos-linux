#include <iostream> // for std::cout, std::endl

/*
 * This is a simple example that spends lots of time in a function.
 * You can watch this with 'perf top' and enter the function with 's'.
 *
 *		Mark Veltzer
 */

class A {
	public:
		//static void performance_hog_function() __attribute__((noinline)) {
		static void performance_hog_function() {
			float sum=0;
			for(unsigned int i=0;i<1000000;i++) {
				for(unsigned int j=0;j<1000000;j++) {
					sum+=i*j;
				}
			}
			std::cout << sum << std::endl;
		}
};

int main(int argc, char **argv, char **envp) {
	A::performance_hog_function();
	return(0);
}
