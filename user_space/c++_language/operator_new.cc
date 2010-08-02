#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *      This explores how to override the new operator for the entire C++ environment.
 *      Both for regular allocation and for arrays....
 *
 *      Notes:
 *
 *      the "void operator delete(void* p)" takes care of deleting
 *      both "void* operator new(const unsigned int size)" and
 *      "void* operator new(const unsigned int size,const unsigned int type)" allocated
 *      objects. You cannot pass hints to the delete operator and so one delete operator
 *      takes care of all hinted operators. The same goes for the array case.
 *
 *      if you do not declare an array allocation then:
 *      - for regular/non hinted array allocation the regular C++ array
 *      allocator will be used.
 *      - for non regular/hinted array allocation you'll get a compile time error
 *      (missing an allocation opeator of the type you desire...).
 *
 *      in order to activate delete[] you *MUST* write you release code as:
 *      delete [] arr2. doing "delete my_array" or an array allocated as
 *      "myarray=new obj[x]" will result in the regular delete(void*) operator
 *      being called.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
inline void *operator new(const unsigned int size) {
	void *p = malloc(size);

	fprintf(stderr, "in operator new with size=%d,p=%p\n", size, p);
	return(p);
}


inline void operator delete(void *p) {
	fprintf(stderr, "in operator delete with p=%p\n", p);
	free(p);
}


inline void *operator new(const unsigned int size, const unsigned int type) {
	void *p = malloc(size);

	fprintf(stderr, "in operator new with size=%d,type=%d,p=%p\n", size, type, p);
	return(p);
}


inline void *operator new[] (const unsigned int size) {
	void *p = malloc(size);

	fprintf(stderr, "in operator new[] with size=%d, p=%p\n", size, p);
	return(p);
}


inline void operator delete[] (void *p) {
	fprintf(stderr, "in operator delete[] with p=%p\n", p);
	free(p);
}


inline void *operator new[] (const unsigned int size, const unsigned int type) {
	void *p = malloc(size);

	fprintf(stderr, "in operator new[] with size=%d,type=%d,p=%p\n", size, type, p);
	return(p);
}


inline void operator delete[] (void *p, const unsigned int type) {
	fprintf(stderr, "in operator delete[] with p=%p,type=%d\n", p, type);
	free(p);
}


typedef struct _myobj {
	char data[10];
} myobj;

int main(int argc, char **argv, char **envp) {
	myobj *obj1 = new myobj;

	delete obj1;
	myobj  *obj2 = new(17) myobj;
	delete obj2;
	myobj  *arr1 = new myobj[60];

	delete [] arr1;
	myobj *arr2 = new(17) myobj[60];
	delete [] arr2;
	return(0);
}
