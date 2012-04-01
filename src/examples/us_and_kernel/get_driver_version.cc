#include<stdio.h> // for perror(3), printf(3), snprintf(3)
#include<sys/utsname.h> // for uname(2)
#include<dlfcn.h> // for dlopen(3), dlclose(3)
#include<stdlib.h> // for EXIT_SUCCESS, exit(3), EXIT_FAILURE
#include<us_helper.h> // for CHECK_NOT_M1

/*
 * This example shows how to extract the version number from a "ko" (kernel object)
 * file...
 *
 * BEWARE: This code DOES NOT WORK!!!
 *
 * This code has led me to the conclusion that modinfo does not use dlopen to get
 * the information from the .ko files but rather mmaps them directly and uses
 * 'man 5 elf' to find the format of the executable file. Do NOT use this code - it
 * will crash your program...
 *
 * How to fix this ?
 * Get your hands on the source code of 'modinfo' and copy the relevant parts here.
 * (publish this in a blog for the interested parties...)
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-ldl
 */

void get_driver_version(const char *filename) {
	void* handle = dlopen(filename, RTLD_LAZY);
	if(handle==NULL) {
		perror(dlerror());
		exit(EXIT_FAILURE);
	}
	CHECK_NOT_M1(dlclose(handle));
}

int main(int argc, char **argv, char **envp) {
	struct utsname buf;
	CHECK_NOT_M1(uname(&buf));
	printf("os verions is %s\n", buf.release);
	char filename[256];
	snprintf(filename,256,"/lib/modules/%s/kernel/drivers/ata/ahci.ko", buf.release);
	printf("filename is %s\n", filename);
	get_driver_version(filename);
	return EXIT_SUCCESS;
}
