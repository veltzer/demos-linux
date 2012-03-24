#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

#include <sys/utsname.h>
#include <dlfcn.h>

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

#include "us_helper.hh"

void get_driver_version(const char *filename) {
	void* handle = dlopen(filename, RTLD_LAZY);
	if(handle==NULL) {
		perror(dlerror());
		exit(1);
	}
	CHECK_NOT_M1(dlclose(handle));
}


int main(int argc, char **argv, char **envp) {
	struct utsname buf;

	CHECK_NOT_M1(uname(&buf));
	printf("os verions is %s\n", buf.release);
	char filename[256];
	sprintf(filename, "/lib/modules/%s/kernel/drivers/ata/ahci.ko", buf.release);
	printf("filename is %s\n", filename);
	get_driver_version(filename);

	return(0);
}
