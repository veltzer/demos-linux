#include<stdlib.h> // for atoi(3)
#include<string.h> // for memset(3)

#include"us_helper.hh" // for my_system

/*
 * This example explores how static data (data segement) affects binary size
 * and speed
 *
 * Notes:
 * - if you initialize the array to 0 (first example) then the binary is small.
 * - if you do not initialize the array (second example) then the binary is small.
 * - if you initialize the array to anything which is not 0 then the binary is large.
 *
 * 					Mark Veltzer
 */

const unsigned int size=1000000;
// first example
//int myarray[size]={ 0 };
// second example
//int myarray[size];
// third example
int myarray[size]={ 3 };

int main(int argc,char** argv,char** envp) {
	unsigned int sum=0;
	for(int i=0;i<argc;i++) {
		sum+=atoi(argv[i]);
	}
	memset(myarray,sum,sizeof(int)*size);
	sum=0;
	for(unsigned int l=0;l<size;l++) {
		sum+=myarray[l];
	}
	TRACE("sum is %u\n",sum);
	my_system("ls -l %s",argv[0]);
	return EXIT_SUCCESS;
}
