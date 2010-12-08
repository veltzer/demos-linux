#include "us_helper.hh"

/*
 * This example explores compiler builtins like:
 * __FILE__, __LINE__, __FUNCTION__, __PRETTY_FUNCTION__ 
 * Which can be used in order to produce better error/logging/debug/warning messages.
 *
 * 					Mark Veltzer
 */

class A {
	public:
		void thisMethod(int a, int b) {
		}
};

int main(int argc, char **argv, char **envp) {
	printf("__FILE__ is %s\n",__FILE__);
	return 0;
}
