#include <stdio.h> // for printf(3)

/*
 *      This shows how to use preprocessor defines...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
#ifdef __GNUC__
	printf("This means we are in GNUC context\n");
#endif // __GNUC__
#ifdef __cplusplus
	printf("This means that we are in c++ context\n");
#endif // __cplusplus
	return(0);
}
