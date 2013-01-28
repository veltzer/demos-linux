/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <stdio.h> // for printf(3)
#include <unistd.h> // for write(2), _exit(2)
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/syscall.h> // for syscall(2)
#include <string.h> // for strlen(3)

int func(int a, int b) {
	return(a + b);
}

/*
void do_init(void) __attribute__((constructor));
void do_init(void) {
	printf("Hello\n");
}
*/

const char other_buf[]="Hello world from the lib\n";

//int main(int argc,char** argv,char** envp) __attribute__((weak,noreturn));
//int main(int argc,char** argv,char** envp) __attribute__((weak));
//int mymain(int argc,char** argv,char** envp) __attribute__((noreturn));
int mymain(int argc,char** argv,char** envp) {
	//const char buf_data[]="Hello world from the lib\n";
	//const int len=strlen(buf_data);
	// this fails
	//printf("hello from the lib...\n");
	// this succeeds
	/*
	volatile int i=0;
	while(1) {
		i++;
	}
	*/
	// lets try a write syscall to file descriptor 1...
	//strcpy(mybuf.buf,buf_data);
	//const int len=strlen(buf_data);
	//const char* buf=(const char*)0x700;
	//const char* buf=(const char*)(buf_data);
	//const char* mybuf=buf_data;
	long res;
	// SYS_write is 4 (from /usr/include/i386-linux-gnu/asm/unistd_32.h)
	// file descriptor is 1
	// buffer
	// size of buffer
	// enter kernel mode
	/*
	__asm__ volatile(			\
		"mov $4,%eax\n\t"		\
		"mov $1,%ebx\n\t"		\
		"mov %0,%ecx\n\t"		\
		"mov %1,%edx\n\t"		\
		"int $0x80\n\t"			\
		: "=a" (res)			\
		:"r" ((long)(buf_data)), "r" ((long)(len))	\
	);
	*/
	__asm__ volatile(						\
		"int $0x80"						\
		: "=a" (res)						\
		: "0" (4),"b" ((long)(1)),"c" ((long)(other_buf)),	\
		"d" ((long)(5))						\
	);
	// SYS_exit is 1 (from /usr/include/i386-linux-gnu/asm/unistd_32.h)
	// argument is in ebx, it is 0
	//"xor %ebx,%ebx\n\t"
	// enter kernel mode
	__asm__ volatile(			\
		"mov $1,%eax\n\t"		\
		"mov $7,%ebx\n\t"		\
		"int $0x80\n\t"			\
	);
	// this works!!!
	/*
	asm(
		// SYS_exit is 1 (from /usr/include/i386-linux-gnu/asm/unistd_32.h)
		"mov $1,%eax;"
		// argument is in ebx, it is 0
		"xor %ebx,%ebx;"
		// enter kernel mode
		"int $0x80;"
	);
	*/
	// call syscall using assembly code
	// this fails
	//syscall(SYS_exit);
	// this fails
	//_exit(EXIT_FAILURE);
	/*
	int ret=write(1,"hello\n",6);
	if(ret!=6) {
		_exit(EXIT_FAILURE);
	}
	*/
	// even this fails
	return EXIT_SUCCESS;
}
