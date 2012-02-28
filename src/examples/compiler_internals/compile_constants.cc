#include <stdlib.h> // for atoi(3)
#include <iostream> // for std::cerr, std::endl, 

/*
 * This example shows how to use compile time constants which
 * are known by the compiler in order to speed things up and
 * still present one API.
 *
 *              Mark Veltzer
 *
 * TODO:
 * - this example does not work. Fix it up.
 */
static inline int _sum(int i) {
	std::cerr << "in _sum for " << i << std::endl;
	return(i *(i + 1) / 2);
}


/*
 *      Take note that this function must be inlined or in the compilation unit with
 *      it's usage points for this trick to take effect. If you are worried about
 *      what happens when the compiler compiles this function for real (not in inline mode)
 *      then you not need be worried: __builtin_constant_p(x) is then always assumed to
 *      return false and so the entire branch of that code goes away including the 'if'
 *      statement itself!
 */
static inline int sum(int i) {
	//if (__builtin_constant_p(i) && (i == 100)) {
		//return(5051);
	if (__builtin_constant_p(i)) {
		return(5051);
	} else {
		return(_sum(i));
	}
}

void fake_manipulate_int(int *ip) {
	*ip = atoi("100");
}

int main(int argc, char **argv, char **envp) {
	std::cerr << "sum for 99 is " << sum(99) << std::endl;
	int j = 0;
	fake_manipulate_int(&j);
	std::cerr << "sum for j=100 is " << sum(j) << std::endl;
	std::cerr << "sum for 100 is " << sum(100) << std::endl;
	std::cerr << "sum for 100 is " << sum(100) << std::endl;
	std::cerr << "sum for 100 is " << sum(100) << std::endl;
	std::cerr << "sum for 101 is " << sum(101) << std::endl;
	return(0);
}
